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

Object *File_run(const char *name)
{
  assert(Lobby != NULL && "Runtime has not been initialized");
  debug("Executing %s", name);

  bstring filename          = bfromcstr(name);
  BytecodeFile *main_method = BytecodeFile_new(filename);

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

  Machine_destroy(machine);

  return result;
}

int main(int argc, char const *argv[])
{
  if (!argv[1]) die("USAGE: ./vm filename.tvm");
  Runtime_init();

  File_run("kernel/prelude.tvm");
  Object *result = File_run(argv[1]);

#ifndef NDEBUG
  debug("---------- Return value");
  Object_print(result);
#endif

  Object_destroy(result);

  Runtime_destroy();

  return 0;
}
