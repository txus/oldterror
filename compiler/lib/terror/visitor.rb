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

    def local_variable_access(node, parent)
      g.loadlocal node.name
    end

    def block(node, parent)
      node.array.each do |expression|
        expression.lazy_visit self, node
      end
    end

    def nil_literal(node, parent)
      g.loadnil
    end

    def true_literal(node, parent)
      g.loadbool 1
    end

    def false_literal(node, parent)
      g.loadbool 0
    end

    def string_literal(node, parent)
      g.loads node.string
    end

    def send(node, parent)
      rcv  = node.receiver.lazy_visit self, node
      meth = g.loads node.name
      args = meth + 1

      g.send_message rcv, meth, args
    end

    def send_with_arguments(node, parent)
      rcv  = node.receiver.lazy_visit self, node
      meth = g.loads node.name

      args = node.arguments.lazy_visit self

      g.send_message rcv, meth, args.first
    end

    def actual_arguments(node, parent)
      slots = []
      body = node.array
      body.each_with_index do |argument, index|
        slots << argument.lazy_visit(self, parent)
      end
      slots
    end

    def self(node, parent)
      g.loadself
    end

    def if(node, parent)
      condition = node.condition.lazy_visit(self, parent)

      body_val = else_val = nil

      done = g.new_label
      else_label = g.new_label

      g.jif else_label, condition

      body_val = node.body.lazy_visit(self, parent)
      g.jmp done

      else_label.set!

      else_val = node.else.lazy_visit(self, parent)

      done.set!
      else_val || body_val
    end

    def finalize
      g.encode
    end
  end
end
