$: << 'lib'
require 'terror/generator'
require 'minitest/autorun'

module Terror
  describe Generator do
    before do
      @g = Generator.new
    end

    describe 'branching labels' do
      it 'works' do
        @g.loadi(3)
        label = @g.new_label
        @g.loadi(3)
        @g.jmp label
        @g.loadi(3)
        @g.loadi(3)
        label.set!

        label.offset.must_equal 3
      end
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
        @g.literals.must_equal ["\"hello"]
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

    describe '#jmp' do
      it 'performs an inconditional jump' do
        @g.jmp(@g.new_label)
      end
    end

    describe '#jmp' do
      it 'jumps a instructions if b is falsy' do
        @g.jif(@g.new_label, 0)
      end
    end

    describe '#jit' do
      it 'jumps to a label if b is truthy' do
        @g.jit(@g.new_label, 0)
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
        @g.locals.push Generator::Local.new(:foo, 3)
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

    describe '#loadslot' do
      it 'loads a slot from an object' do
        slot = @g.loadslot 1, :foo
        @g.registers[0].value.must_equal :foo
        slot.must_equal 0
      end
    end

    describe '#setslot' do
      it 'loads a slot from an object' do
        @g.setslot 0, :foo, 1
      end
    end

    describe '#makearray' do
      it 'makes an array from a collection of objects' do
        @g.makearray 1, 3
        @g.registers[0].value.must_equal :array
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
