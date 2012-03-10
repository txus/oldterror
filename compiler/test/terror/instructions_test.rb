$: << 'lib'
require 'terror/instructions'
require 'minitest/autorun'

module Terror
  describe Instructions do
    before do
      @g = Class.new { include Instructions }.new
    end

    describe 'instructions with 3 operands' do
      %w(add sub mul div send).each do |instruction|
        describe instruction do
          it "#{instruction} is encoded correctly" do
            inst = @g.__send__ :"_#{instruction}", 2, 3, 4
            value = '%0x' % inst.class.value

            inst.operands.must_equal [2, 3, 4]
            inst.encode.must_equal "#{value}020304"
          end
        end
      end
    end

    describe 'instructions with 2 operands' do
      %w(move loadi loads loadbool loadlocal setlocal).each do |instruction|
        describe instruction do
          it "#{instruction} is encoded correctly" do
            inst = @g.__send__ :"_#{instruction}", 2, 3
            value = '%0x' % inst.class.value
            # Hack for :move (0x00)
            if value == '0'
              value = ''
            else
              value += '0'
            end

            inst.operands.must_equal [2, 3]
            inst.encode.must_equal "#{value}20300"
          end
        end
      end
    end

    describe 'instructions with 1 operand' do
      %w(loadnil loadself).each do |instruction|
        describe instruction do
          it "#{instruction} is encoded correctly" do
            inst = @g.__send__ "_#{instruction}", 2
            value = '%0x' % inst.class.value

            inst.operands.must_equal [2]
            inst.encode.must_equal "#{value}020000"
          end
        end
      end
    end

    describe 'instructions with no operands' do
      %w(ret dump).each do |instruction|
        describe instruction do
          it "#{instruction} is encoded correctly" do
            inst = @g.__send__ :"_#{instruction}"
            value = '%0x' % inst.class.value

            inst.operands.must_equal []
            inst.encode.must_equal "#{value}000000"
          end
        end
      end
    end
  end
end
