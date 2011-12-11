CFLAGS=-Wall -g

all: vm
	cp vm ../vm

vm: object.o stack.o runtime.o vmmethod.o machine.o slot.o gc.o util.o

clean:
	rm -f ../vm
	rm -f *.o
	rm -fR *.dSYM
