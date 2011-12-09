# TerrorVM
This will eventually become a stack-based VM running under the [NoScript](http://github.com/txus/noscript)
language.


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
