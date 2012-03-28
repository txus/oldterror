require 'terror/instructions'
require 'terror/allocator'

module Terror
  class Generator
    include Instructions
    attr_reader :literals

    def initialize
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

    private

    def register_error(num)
      raise "There's nothing on register #{num}"
    end

    def literal value
      @literals.index(value) or begin
        @literals.push value
        @literals.index(value)
      end
    end
  end
end
