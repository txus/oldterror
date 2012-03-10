# TerrorVM

A register-based, garbage-collected, stackless, lightweight Virtual Machine
for object-oriented programming languages.

Ships with a naive implementation of a reference-counting Garbage Collector,
although it will implement a more advanced GC algorithm, probably Baker's
treadmill.

Eventually the VM will be optimized to be fast on ARM processors, but for now
it compiles to both ARM and x86 architectures. It will also use LLVM for
JIT-compiling code to native at runtime.

Anyway, it's a work in progress! :)

## Building the VM

For now it comes with a default program whose instructions are hardcoded inside
`vm.c`.

    $ git clone git://github.com/txus/terrorvm.git
    $ cd terrorvm
    $ make
    $ ./vm

To run the tests:

    $ make test

And to clean the mess:

    $ make clean

## Bytecode format

Instructions have a compact 3-operand representation, 8-bit each, for a total
of 32-bit per instruction.
