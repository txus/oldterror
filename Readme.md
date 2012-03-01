# TerrorVM
This will eventually become a register-based VM for ARM architectures.

In the branch `old`, there's the version where it's a very basic stack-based VM,
compiling on both ARM and x86.

# Building the VM

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

Anyway, it's a work in progress!
