require 'terror/generator'

module Terror
  class Visitor
    attr_reader :generator

    def initialize(g=Generator.new)
      @generator = g
      @slots = {}
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
      if node.receiver.respond_to?(:name) && @slots[node.receiver.name] && @slots[node.receiver.name].include?(node.name)
        return slot_retrieval(node, parent)
      end

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

    def slot_retrieval(node, parent)
      receiver = node.receiver.lazy_visit self
      name     = g.loads node.name
      g.loadslot receiver, name
    end

    def attribute_assignment(node, parent)
      receiver_name = node.receiver.name
      attribute_name = node.name[0..-2].to_sym
      @slots[receiver_name] ||= []
      @slots[receiver_name] << attribute_name

      receiver = node.receiver.lazy_visit self
      name     = g.loads attribute_name
      value    = node.arguments.array.first.lazy_visit self
      g.setslot receiver, name, value
    end

    def array_literal(node, parent)
      first = nil
      node.body.each do |element|
        slot = element.lazy_visit self
        # Save the register of only the first element
        first ||= slot
      end
      g.makearray first, node.body.count
    end

    def empty_array(node, parent)
      g.makearray 0, 0
    end

    def hash_literal(node, parent)
      slf  = g.loadself
      meth = g.loads :hash

      # Create an array
      first = nil
      node.array.each do |element|
        slot = element.lazy_visit self
        # Save the register of only the first element
        first ||= slot
      end
      ary = g.makearray first, node.array.count

      g.send_message slf, meth, ary
    end

    def symbol_literal(node, parent)
      g.loads node.value
    end

    def finalize
      g.encode
    end
  end
end
