require 'terror/generator'

module Terror
  class Visitor
    attr_reader :generator

    def initialize(g=Generator.new)
      @generator = g
    end
    alias_method :g, :generator

    def fixnum_literal(node, parent)
      g.loadi node.value
    end

    def local_variable_assignment(node, parent)
      register = node.value.lazy_visit self, node
      g.setlocal node.name, register
      register
    end

    def block(node, parent)
      node.array.each do |expression|
        expression.lazy_visit self, node
      end
    end

    def finalize
      g.disassemble
    end
  end
end
