$: << 'lib'
require 'terror/allocator'
require 'minitest/autorun'

describe Allocator do
  before do
    @allocator = Allocator.new
  end

  describe 'when there are free registers' do
    it 'allocates values' do
      @allocator.allocate(:foo).must_equal 0
    end
  end

  describe 'when there are no free registers' do
    it 'allocates values' do
      @allocator.instance_eval do 
        reg = Allocator::Register.new
        reg.value = :bar
        @registers = [reg]
      end
      @allocator.allocate(:foo).must_equal 1
    end
  end

  describe 'when called with a specific position' do
    it 'allocates values' do
      @allocator.allocate(:foo, 7).must_equal 7
    end
  end

  describe Allocator::Register do
    before do
      @register = Allocator::Register.new
    end

    describe 'when it has a value' do
      before do
        @register.value = :foo
      end

      describe 'and it is not discarded' do
        it 'is not free' do
          @register.free?.must_equal false
        end
      end

      describe 'but it is discarded' do
        it 'is free' do
          @register.discard
          @register.free?.must_equal true
        end
      end
    end

    describe 'when it does not have a value' do
      it 'is free' do
        @register.free?.must_equal true
      end
    end
  end
end
