CFLAGS=-Wall -g

all: vm

vm: object.o stack.o runtime.o vmmethod.o machine.o

clean:
	rm -f vm
	rm -f *.o
	rm -fR *.dSYM
