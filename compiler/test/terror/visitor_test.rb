$: << 'lib'
require 'terror/visitor'
require 'terror/core_ext/node'
require 'minitest/autorun'

module Terror
  describe Visitor do
    def visit(code)
      ast = Rubinius::Melbourne19.parse_string(code)
      visitor = Visitor.new
      ast.lazy_visit(visitor, ast, true)
      visitor.generator
    end

    it 'visits fixnums' do
      p visit("3")
    end
  end
end
