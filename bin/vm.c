#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <terror/util.h>
#include <terror/dbg.h>
#include <terror/input_reader.h>
#include <terror/machine.h>
#include <terror/runtime.h>

// Extern global objects declared in runtime.h
Object *TrueObject;
Object *FalseObject;
Object *NilObject;
Object *Lobby;

int main(int argc, char const *argv[])
{
  if (!argv[1]) die("USAGE: ./vm filename.tvm");
  bstring filename          = bfromcstr(argv[1]);
  BytecodeFile *main_method = BytecodeFile_new(filename);

  Runtime_init();

  Object *locals[main_method->locals_count];

  Machine *machine = Machine_new(
    main_method->instructions,
    main_method->instructions_count,
    main_method->literals,
    main_method->literals_count,
    locals,
    main_method->locals_count,
    main_method->registers_count
    );

  Object *result = Machine_run(machine, Lobby);

#ifndef NDEBUG
  debug("---------- Return value");
  Object_print(result);
#endif

  Object_destroy(result);
  Machine_destroy(machine);

  Runtime_destroy();

  return 0;
}
