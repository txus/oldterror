require 'terror/instructions'

module Terror
  class Generator
    include Instructions
    def initialize
      @registers = []
      @literals  = []
    end

    def disassemble
      instructions.map(&:to_s).join "\n"
    end

    def loadi register, value
      _loadi literal(value)
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
