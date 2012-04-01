require 'terror/instructions'
require 'terror/allocator'

module Terror
  class Generator
    include Instructions
    attr_reader :literals, :locals

    Local = Struct.new(:name, :value)

    def initialize
      @locals    = []
      @literals  = []
      @allocator = Allocator.new
    end

    def a
      @allocator
    end

    def registers
      @allocator.registers
    end

    def disassemble
      instructions.map(&:to_s).join "\n"
    end

    def loadi(value)
      lit = literal(value)
      slot = a.allocate lit
      _loadi slot, lit
      slot
    end

    def loads(value)
      lit = literal(value)
      slot = a.allocate lit
      _loads slot, lit
      slot
    end

    def loadnil
      slot = a.allocate :nil
      _loadnil slot
      slot
    end

    def loadbool(which)
      slot = a.allocate [:false, :true][which]
      _loadbool slot, which
      slot
    end

    def move(b)
      register_error b unless registers[b]

      slot = a.allocate registers[b].value
      _move slot, b
      slot
    end

    %w(add sub mul div).each do |op|
      define_method(op) do |b, c|
        register_error b unless registers[b]
        register_error c unless registers[c]

        slot = a.allocate op.to_sym
        send :"_#{op}", b, c
        slot
      end
    end

    def jmp(a)
      _jmp a
    end

    def jif(a, b)
      _jif a, b
    end

    def jit(a, b)
      _jit a, b
    end

    def loadself
      slot = a.allocate :self
      _loadself slot
      slot
    end

    def loadlocal(name)
      slot = a.allocate :local
      _loadlocal slot, local(name)
      slot
    end

    def setlocal(name, b)
      idx = local(name, b)
      @locals[idx].value = b
      _setlocal b, idx
      idx
    end

    def send_message(a, b, c)
      _send a, b, c
      a
    end

    def ret(a)
      _ret a
      a
    end

    private

    def register_error(num)
      raise "There's nothing on register #{num}"
    end

    def local name, value=nil
      @locals.index { |l| l.name == name } or begin
        @locals.push Local.new name, value
        @locals.size - 1
      end
    end

    def literal value
      @literals.index(value) or begin
        @literals.push value
        @literals.index(value)
      end
    end
  end
end
