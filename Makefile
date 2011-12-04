CFLAGS=-Wall -g

all: vm

vm: object.o stack.o runtime.o

clean:
	rm -f vm
	rm -f *.o
	rm -fR *.dSYM
