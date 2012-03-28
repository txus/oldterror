class Allocator
  def initialize
    @registers = Array.new(10) { Register.new }
  end

  def size
    @registers.size
  end

  def allocate(value, at=nil)
    return allocate_value_at(value, at) if at
    register = @registers.select { |r| r.free? }.first || grow
    register.value = value
    @registers.index register
  end

  private

  def grow
    register = Register.new
    @registers.push register
    register
  end

  def allocate_value_at(value, at)
    @registers[at] = value
    at
  end

  class Register
    attr_accessor :value

    def discard
      @free = true
    end

    def free?
      @free || !@value
    end
  end
end
