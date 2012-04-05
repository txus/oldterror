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

void run(long literals[], Instruction instructions[]) {
  // Instruction pointer
}

int main(int argc, char const *argv[])
{
  if (!argv[1]) die("USAGE: ./vm filename.tvm");

  BytecodeFile *main_method = load_file("something.tvm");

  /* long literals[] = { */
  /*   (long) 30, */
  /*   (long) "this is a string test, pretty long by the way. do you think it will fit in a (long)?", */
  /*   (long) "print", */
  /*   (long) "add" */
  /* }; */

  /* uint32_t encoded_instructions[] = { */
  /*   OP_LOADI(1, 0), */
  /*   OP_LOADI(2, 0), */
  /*   OP_ADD(0, 1, 2), */
  /*   OP_MOVE(4, 0), */
  /*   OP_SETLOCAL(4, 0), */
  /*   OP_LOADNIL(4), */
  /*   OP_LOADI(0, 0), */
  /*   OP_JIT(3, 0), */
  /*   OP_LOADS(7, 3), */
  /*   OP_LOADS(9, 3), */
  /*   OP_LOADS(1, 3), */
  /*   OP_SEND(0, 1, 2), */
  /*   OP_LOADLOCAL(7, 0), */
  /*   OP_DUMP(), */
  /*   OP_RET(0), */
  /* }; */

  /* Instruction instructions[20]; */

  /* int i = 0; */
  /* int length = sizeof(encoded_instructions) / sizeof(uint32_t); */
  /* for(i=0; i < length; i++) { */
  /*   instructions[i] = Instruction_new(encoded_instructions[i]); */
  /* } */

  init_runtime();

  Object *locals[main_method->locals_count];
  Machine *machine = Machine_new(main_method->instructions, main_method->literals, locals);
  Object *result = Machine_run(machine, MainObject);
  Object_destroy(result);
  Machine_destroy(machine);

  destroy_runtime();

  return 0;
}
