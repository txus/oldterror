CFLAGS=-Wall -g

all:
	cd src; make

test: test-compile
	./test/test
	./test/leaks vm && ./test/leaks test/test

test-compile:
	./test/clay.py test
	gcc test/clay_main.c src/util.c src/machine.c src/instruction.c src/object.c src/gc.c src/slot.c src/vmmethod.c src/registers.c src/runtime.c test/*_test.c -o test/test -Wall -g

clean:
	rm -f test/clay.h test/clay_main.c test/test
	cd src; make clean
