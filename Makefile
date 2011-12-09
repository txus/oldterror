CFLAGS=-Wall -g

all:
	cd src; make

test: test-compile
	./test/test

test-compile: all
	./test/clay.py test
	gcc test/clay_main.c test/*_test.c -o test/test

clean:
	rm test/clay.h
	rm test/clay_main.c
	rm test/test
	cd src; make clean
