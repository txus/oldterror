#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "dbg.h"
#include "util.h"
#include "input_reader.h"
#include "opcode.h"
#include "instruction.h"
#include "object.h"
#include "registers.h"
#include "runtime.h"
#include "machine.h"

// Extern global objects declared in runtime.h
Object *TrueObject;
Object *FalseObject;
Object *NilObject;
Object *MainObject;

int main(int argc, char const *argv[])
{
  if (!argv[1]) die("USAGE: ./vm filename.tvm");

  BytecodeFile *main_method = load_file(argv[1]);

  init_runtime();

  Object *locals[main_method->locals_count];
  Machine *machine = Machine_new(main_method->instructions, main_method->literals, locals);
  Object *result = Machine_run(machine, MainObject);
  Object_destroy(result);
  Machine_destroy(machine);

  destroy_runtime();

  return 0;
}
