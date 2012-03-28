$: << 'lib'
require 'terror/generator'
require 'minitest/autorun'

module Terror
  describe Generator do
    before do
      @g = Generator.new
    end

    describe '#loadi' do
      it 'loads an integer into the first free register' do
        @g.loadi(3).must_equal 0
        @g.literals.must_equal [3]
        @g.registers[0].value.must_equal 0
      end
    end

    describe '#move' do
      it 'moves a value between registers' do
        @g.move(1, 0).must_equal 1
      end
    end
  end
end
