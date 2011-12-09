CFLAGS=-Wall -g

all: vm
	mv vm ../

vm: object.o stack.o runtime.o vmmethod.o machine.o slot.o

clean:
	rm -f ../vm
	rm -f *.o
	rm -fR *.dSYM
