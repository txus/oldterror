module Terror
  module Instructions
    module Instruction
      # Internal: Add a :value accessor to the host class when included.
      def self.included(base)
        class << base; attr_accessor :value; end
      end

      # Public: encodes an Instruction into a compact 32-bit format.
      #
      # Returns a String encoded in hexadecimal format.
      def encode
        "%0x" % (
          (self.class.value << 24) +
          (a << 16) +
          (b << 8) +
          c
        )
      end

      # Public: Disassembles the instruction to a readable format.
      #
      # Returns a readable String representing the instruction and its operands.
      def to_s
        output = [
          self.class.name.upcase,
          self.class.value
        ] + operands

        output.join " "
      end

      # Internal: Returns operand a or zero.
      def a; @a || 0x0; end

      # Internal: Returns operand b or zero.
      def b; @b || 0x0; end

      # Internal: Returns operand c or zero.
      def c; @c || 0x0; end

      # Internal: Returns the operands of the instruction.
      def operands
        length = if respond_to?(:members)
                   members.length
                 else
                   0
                 end
        [:a, :b, :c].take(length).map do |operand|
          __send__ operand
        end
      end
    end

    # Internal: Little DSL helper that generates a method for a given opcode,
    # value and arity.
    #
    # name  - the Symbol opcode name (:move, :loadi...)
    # value - the Fixnum opcode value (0x00, 0x01...)
    # arity - the Fixnum number of operands that the instruction accepts
    #
    # Example
    #
    #   op :move, 0x00, 2
    #
    #   # Generates:
    #   klass = Class.new Struct.new(:a, :b)
    #   klass.value # => 0x00
    #   define_method(:move) do |*args|
    #     Move.new(*args)
    #   end
    #
    def self.op(name, value, arity)
      operands = [:a, :b, :c].take(arity)

      if arity > 0
        klass = Class.new(Struct.new(*operands)) { include Instruction }
      else
        klass = Class.new { include Instruction }
      end
      klass.value = value

      Instructions.const_set name.to_s.capitalize, klass

      define_method(:"_#{name}") do |*args|
        instruction = Instructions.const_get(name.capitalize)
        if arity > 0
          emit instruction.new(*args)
        else
          emit instruction.new
        end
      end
    end

    # Internal: Rmits an instruction.
    #
    # instruction - the Instruction to emit.
    #
    # Returns nothing.
    def emit(instruction)
      instructions << instruction
      instruction
    end

    # op :opcode, value, arity
    op :move,      0x00, 2
    op :loadi,     0x01, 2
    op :loads,     0x02, 2

    op :loadnil,   0x08, 1
    op :loadbool,  0x09, 2

    op :add,       0x10, 3
    op :sub,       0x11, 3
    op :mul,       0x12, 3
    op :div,       0x13, 3

    op :loadself,  0x30, 1

    op :loadlocal, 0x50, 2
    op :setlocal,  0x51, 2

    op :send,      0x80, 3
    op :ret,       0x90, 1
    op :dump,      0x91, 0

    # Public: Returns the Array of emitted instructions.
    def instructions
      @instructions ||= []
    end
  end
end
