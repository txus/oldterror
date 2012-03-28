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

    def move(a, b)
      _move a, b
      a
    end

    private

    def literal value
      @literals.index(value) or begin
        @literals.push value
        @literals.index(value)
      end
    end
  end
end
