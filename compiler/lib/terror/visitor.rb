require 'terror/generator'

module Terror
  class Visitor
    attr_reader :generator

    def initialize(g=Generator.new)
      @generator = g
      @registers = []
    end
    alias_method :g, :generator

    def method_missing(m, *args, &block)
      puts "VISITOR: ignored #{m}"
    end

    def fixnum_literal(node, parent)
      g.loadi node.value
      emit node.value.to_s
    end

    def finalize
      g.disassemble
    end
  end
end
