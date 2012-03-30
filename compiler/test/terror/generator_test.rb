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

    describe '#loads' do
      it 'loads a string literal into the first free register' do
        @g.loads("hello").must_equal 0
        @g.literals.must_equal ["hello"]
        @g.registers[0].value.must_equal 0
      end
    end

    describe '#loadnil' do
      it 'loads nil' do
        @g.loadnil.must_equal 0
        @g.registers[0].value.must_equal :nil
      end
    end

    describe '#loadbool' do
      it 'loads a false boolean' do
        @g.loadbool(0).must_equal 0
        @g.registers[0].value.must_equal :false
      end

      it 'loads a true boolean' do
        @g.loadbool(1).must_equal 0
        @g.registers[0].value.must_equal :true
      end
    end

    describe '#move' do
      it 'moves a value between registers' do
        @g.loadi 42
        @g.move(0).must_equal 1
        @g.registers[1].value.must_equal 0
      end
    end
    
    %w(add sub mul div).each do |op|
      describe "##{op}" do
        it "performs a #{op} operation" do
          @g.loadi 1
          @g.loadi 2
          @g.send op, 0, 1
          @g.registers[2].value.must_equal op.to_sym
        end
      end
    end

    describe '#loadself' do
      it 'loads self' do
        @g.loadself.must_equal 0
        @g.registers[0].value.must_equal :self
      end
    end

    describe '#loadlocal' do
      it 'loads a local variable' do
        @g.locals.push :foo
        @g.loadlocal 0.must_equal 0
        @g.registers[0].value.must_equal :local
      end
    end

    describe '#setlocal' do
      it 'sets a local variable' do
        @g.loadi 42
        @g.setlocal :foo, 0
        local = @g.locals[0]
        local.name.must_equal :foo
        local.value.must_equal 0
      end
    end

    describe '#send' do
      it 'sends a message to an object' do
        @g.send_message(0, 1, 2).must_equal 0
      end
    end

    describe '#ret' do
      it 'returns from the active call frame' do
        @g.ret(0).must_equal 0
      end
    end
  end
end
