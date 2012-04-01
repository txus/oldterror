# TerrorVM

![TerrorVM](https://github.com/txus/terrorvm/raw/master/assets/tvm_large.png)

A register-based, garbage-collected, stackless, lightweight Virtual Machine
for object-oriented programming languages.

Ships with a naive implementation of a reference-counting Garbage Collector,
although it will implement a more advanced GC algorithm, probably Baker's
treadmill.

Eventually the VM will be optimized to be fast on ARM processors, but for now
it compiles to both ARM (tested on Android) and x86 architectures. It will also
use LLVM for JIT-compiling code to native at runtime.

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

## Building for Android

TerrorVM can also be built for Android devices. You'll need to install
the Android NDK 5c (latest 7 doesn't work), and export the environment variable
`ANDROID_NDK` to where you installed it.

    $ make -f Makefile.android

## Bytecode format

Instructions have a compact 3-operand representation, 8-bit each, for a total
of 32-bit per instruction.

## Who's this

This was made by [Josep M. Bach (Txus)](http://txustice.me) under the MIT
license. I'm [@txustice](http://twitter.com/txustice) on twitter (where you
should probably follow me!).
