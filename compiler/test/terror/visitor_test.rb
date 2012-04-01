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
  end
end
