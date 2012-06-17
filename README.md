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

    $ git clone git://github.com/txus/terrorvm.git
    $ cd terrorvm
    $ make

To run the tests:

    $ make dev

And to clean the mess:

    $ make clean

## Running programs

TerrorVM runs `.tvm` bytecode files such as the `hello_world.tvm` under the
`examples` directory.

    $ ./bin/vm examples/hello_world.tvm

It ships with a simple compiler written in Ruby (Rubinius) that compiles a
tiny subset of Ruby to `.tvm` files. Check out the `compiler` directory, which
has its own Readme, and the `compiler/examples` where we have the
`hello_world.rb` file used to produce the `hello_world.tvm`.

TerrorVM doesn't need Ruby to run; even the example compiler is a proof of
concept and could be written in any language (even in C obviously).

## Implementing your own dynamic language running on TerrorVM

TerrorVM is designed to run dynamic languages. You can easily implement a
compiler of your own that compiles your favorite dynamic language down to TVM
bytecode.

I've written a demo compiler in Ruby under the `compiler/` folder, just to
show how easy it is to write your own. This demo compiler compiles a subset of
Ruby down to TerrorVM bytecode, so you can easily peek at the source code or
just copy and modify it.

You can write your compiler in whatever language you prefer, of course.

## Bytecode format

TerrorVM files are encoded with a header containing `_main` (the method
that will be the entry point), some info about number of registers, local
variables, literals and instructions used by the method, followed by all the
literals, and then all the instructions. It starts like this:

    _main

Then info encoded in the format
`:num_registers:num_locals:num_literals:num_instructions`:

    :10:2:4:17

Then all the literals, each one in a line (the ones starting with `"` are
string literals):

    123
    "print
    "Goodbye world!
    "Hello world!

And then all the instructions:

    0x2000000
    0x51000000
    0x9010000
    0x51010100
    ...

Instructions have a compact 3-operand representation, 8-bit each, for a total
of 32-bit per instruction.

## VM primitive functions

TerrorVM exposes a `VM` object that responds to `primitive`, which returns a
hash with some VM primitive functions exposed as Terror Function objects.
A simple example of those are arithmetic functions (`+`, `-`, `*`, `/`) used
by Integer objects, for example. To use this in your functions, do it like
this:

    VM.primitive[:+].apply(3, 4) # this is the same as 3 + 4 or 3.+(4)

### Examples

* Hello world ([Ruby code](https://github.com/txus/terrorvm/blob/master/compiler/examples/hello_world.rb), [TVM bytecode](https://github.com/txus/terrorvm/blob/master/examples/hello_world.tvm))
* Numbers ([Ruby code](https://github.com/txus/terrorvm/blob/master/compiler/examples/numbers.rb), [TVM code](https://github.com/txus/terrorvm/blob/master/examples/numbers.tvm))
* Objects with prototypal inheritance ([Ruby code](https://github.com/txus/terrorvm/blob/master/compiler/examples/objects.rb), [TVM bytecode](https://github.com/txus/terrorvm/blob/master/examples/objects.tvm))
* Exposed VM primitives ([Ruby code](https://github.com/txus/terrorvm/blob/master/compiler/examples/primitives.rb), [TVM bytecode](https://github.com/txus/terrorvm/blob/master/examples/primitives.tvm))

### Instruction set

* **NOOP**: no operation -- does nothing.

#### Loading values

* **MOVE A, B**: copies the contents in register `B` to register `A`.
* **LOADI A, B**: loads the integer (from the literal pool) at index `B` into
  register `A`.
* **LOADS A, B**: loads the string (from the literal pool) at index `B` into
  register `A`.
* **LOADNIL A**: loads the special value `nil` into register `A`.
* **LOADBOOL A, B**: loads a boolean into register `A`, being true if `B` is
  the number 1 or false if `B` is 0.
* **LOADSELF A**: loads the current `self` into register `A`.

#### Branching

* **JMP A**: unconditionally jumps `A` instructions.
* **JIF A, B**: jumps `A` instructions if the contents of the register `B` are
  either `false` or `nil`.
* **JIT A, B**: jumps `A` instructions if the contents of the register `B` are
  neither `false` nor `nil`.

#### Local variables

* **LOADLOCAL A, B**: loads the value in the locals table at index `B` into
  the register `A`.
* **SETLOCAL A, B**: stores the contents of the register `A` in the locals
  table at index `B`.

#### Slots

* **LOADSLOT A, B, C**: loads the slot named `C` from the object `B` to the
  register `A`.
* **SETSLOT A, B, C**: sets the slot named `B` from the object `A` to the
  value in the register `C`.

#### Arrays

* **MAKEARRAY A, B, C**: Takes `C` registers starting from register `B`,
  creates an array with them and stores it in register `A`.

#### Message sending and call frames

* **SEND A, B, C**: send a message specified by the string in the literals
  table at index `B` to the receiver `A`, with N arguments depending on the
  arity of the method, the first argument being in the register `C`.
* **RET A**: return from the current call frame to the caller with the value
  in the register `A`.

#### Debugging

* **DUMP**: Print the contents of all the registers to the standard output.

## Who's this

This was made by [Josep M. Bach (Txus)](http://txustice.me) under the MIT
license. I'm [@txustice](http://twitter.com/txustice) on twitter (where you
should probably follow me!).
