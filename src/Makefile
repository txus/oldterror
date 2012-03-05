CFLAGS=-Wall -g

all: vm
	cp vm ../vm

vm: object.o instruction.o registers.o runtime.o vmmethod.o machine.o slot.o gc.o util.o

clean:
	rm -f ../vm vm
	rm -f *.o
	rm -fR *.dSYM
