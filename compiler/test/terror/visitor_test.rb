$: << 'lib'
require 'terror/visitor'
require 'terror/core_ext/node'
require 'minitest/autorun'

module Terror
  describe Visitor do
    include Instructions
    def visit(code)
      ast = Rubinius::Melbourne19.parse_string(code)
      visitor = Visitor.new
      ast.lazy_visit(visitor, ast)
      visitor.generator
    end

    def compiles(code, &block)
      g = Generator.new
      g.instance_eval(&block)
      visit(code).instructions.must_equal g.instructions
    end

    it 'assigns local variables' do
      compiles("a = 3; b = 3; c = 4") do
        _loadi 0, 0
        _setlocal 0, 0
        _loadi 1, 0
        _setlocal 1, 1
        _loadi 2, 1
        _setlocal 2, 2
      end
    end

    it 'remembers local names' do
      compiles("a = 3; a = 4") do
        _loadi 0, 0
        _setlocal 0, 0
        _loadi 1, 1
        _setlocal 1, 0
      end
    end

    it 'compiles nested assignment' do
      compiles("a = b = 3") do
        _loadi 0, 0
        _setlocal 0, 0
        _setlocal 0, 1
      end
    end

    it 'compiles local variable access' do
      compiles("a = 3; a") do
        _loadi 0, 0
        _setlocal 0, 0
        _loadlocal 1, 0
      end
    end

    it 'compiles boolean literals' do
      compiles("true") do
        _loadbool 0, 1
      end

      compiles("false") do
        _loadbool 0, 0
      end

      compiles("nil") do
        _loadnil 0
      end
    end

    it 'compiles string literals' do
      compiles("'hey'") do
        _loads 0, 0
      end
    end

    it 'compiles messages without arguments' do
      compiles("a") do
        _loadself 0
        _loads 1, 0
        _send 0, 1, 2
      end
    end

    it 'compiles messages with arguments' do
      compiles("puts 1, 2, 3") do
        _loadself 0
        _loads 1, 0
        _loadi 2, 1
        _loadi 3, 2
        _loadi 4, 3
        _send 0, 1, 2
      end
    end

    describe 'branching' do
      it 'compiles if' do
        compiles("if 1 then 3; end") do
          _loadi 0, 0
          _jif 3, 0
          _loadi 1, 1
          _jmp 2
          _loadnil 2
        end
      end

      it 'compiles if-else' do
        compiles("if 1 then 3 else 4 end") do
          _loadi 0, 0 # condition
          _jif 3, 0
          _loadi 1, 1 # body
          _jmp 2
          _loadi 2, 2 # else body
        end
      end
    end

    describe 'slots' do
      it 'compiles objects with setters' do
        compiles("a = 3; a.foo = 9") do
          _loadi 0, 0
          _setlocal 0, 0
          _loadlocal 1, 0
          _loads 2, 1
          _loadi 3, 2
          _setslot 1, 2, 3
        end
      end

      it 'compiles objects with getters and setters' do
        compiles("a = 3; a.foo = 9; a.foo") do
          # a = 3
          _loadi 0, 0
          _setlocal 0, 0

          # a.foo = 9
          _loadlocal 1, 0
          _loads 2, 1
          _loadi 3, 2
          _setslot 1, 2, 3

          # a.foo
          _loadlocal 4, 0
          _loads 5, 1
          _loadslot 6, 4, 5
        end
      end
    end

    describe 'arrays' do
      it 'compiles arrays' do
        compiles("[1, 'hey', 3]") do
          _loadi 0, 0
          _loads 1, 1
          _loadi 2, 2
          _makearray 3, 0, 3
        end
      end
    end

    describe 'hashes' do
      it 'compiles hashes' do
        compiles("{a: 123, b: 'hey'}") do
          _loadself 0
          _loads 1, 0 # 'hash'

          _loads 2, 1
          _loadi 3, 2
          _loads 4, 3
          _loads 5, 4
          _makearray 6, 2, 4

          _send 0, 1, 6
        end
      end
    end

    describe 'constants' do
      it 'are compiled down to normal identifiers' do
        compiles("Object.clone") do
          _loadself 0
          _loads 1, 0 # 'Object'
          _loadslot 2, 0, 1

          _loads 3, 1 # 'clone'
          _send 2, 3, 4
        end
      end
    end
  end
end
