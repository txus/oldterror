CFLAGS=-Wall -g

all:
	cd src; make

test: test-compile
	./test/test

test-compile:
	./test/clay.py test
	gcc test/clay_main.c src/machine.c src/object.c src/runtime.c src/slot.c src/stack.c src/vmmethod.c test/*_test.c -o test/test

clean:
	rm -f test/clay.h test/clay_main.c test/test
	cd src; make clean
