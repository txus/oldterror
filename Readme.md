# TerrorVM
This will eventually become a register-based VM optimized to run on ARM
architectures.

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
