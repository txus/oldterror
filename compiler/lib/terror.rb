require "terror/version"
require "terror/core_ext/node"
require "terror/visitor"

module Terror
  def self.parse_file(file)
    AST = Rubinius::Melbourne19.parse_file(ARGV[0])
    visitor = Terror::Visitor.new
    AST.lazy_visit(visitor, AST, true)
    visitor.finalize
  end
end
