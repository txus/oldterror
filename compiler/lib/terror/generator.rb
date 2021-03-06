require 'terror/instructions'
require 'terror/branching'
require 'terror/allocator'

module Terror
  class Generator
    include Instructions
    include Branching
    attr_reader :literals, :locals, :ip

    Local = Struct.new(:name, :value)

    def initialize
      @locals    = []
      @literals  = []
      @ip        = 0
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

    def encode
      # Always return nil at the end of a script
      reg = loadnil
      ret reg

      output = "_main\n"
      output << ":%i:%i:%i:%i\n" % [
        registers.size,
        @locals.size,
        @literals.size,
        @ip
      ]
      output << @literals.join("\n")
      output << "\n"
      # instructions.each do |i|
      #   p i
      # end
      output << instructions.map(&:encode).join("\n")
      output
    end

    def loadi(value)
      @ip += 1

      lit = literal(value)
      slot = a.allocate lit
      _loadi slot, lit
      slot
    end

    def loads(value)
      @ip += 1

      lit = literal("\"#{value}")
      slot = a.allocate lit
      _loads slot, lit
      slot
    end

    def loadnil
      @ip += 1

      slot = a.allocate :nil
      _loadnil slot
      slot
    end

    def loadbool(which)
      @ip += 1

      slot = a.allocate [:false, :true][which]
      _loadbool slot, which
      slot
    end

    def move(b)
      @ip += 1

      register_error b unless registers[b]

      slot = a.allocate registers[b].value
      _move slot, b
      slot
    end

    def jmp(label)
      label.start!
      @ip += 1
      _jmp label
    end

    def jif(label, b)
      label.start!
      @ip += 1
      _jif label, b
    end

    def jit(label, b)
      label.start!
      @ip += 1
      _jit label, b
    end

    def loadself
      @ip += 1
      slot = a.allocate :self
      _loadself slot
      slot
    end

    def loadlocal(name)
      @ip += 1
      slot = a.allocate :local
      _loadlocal slot, local(name)
      slot
    end

    def setlocal(name, b)
      @ip += 1
      idx = local(name, b)
      @locals[idx].value = b
      _setlocal b, idx
      idx
    end

    def loadslot(receiver, name)
      @ip += 1
      slot = a.allocate name
      _loadslot slot, receiver, name
      slot
    end

    def setslot(receiver, name, value)
      @ip += 1
      _setslot receiver, name, value
    end

    def makearray(start, count)
      @ip += 1
      slot = a.allocate :array
      _makearray slot, start, count
      slot
    end

    def send_message(a, b, c)
      @ip += 1
      _send a, b, c
      a
    end

    def ret(a)
      @ip += 1
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
