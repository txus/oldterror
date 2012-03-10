#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "dbg.h"
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
  Instruction *ip = instructions;
  Object *locals[NUM_REGISTERS];

  Machine *machine = Machine_new(ip, literals, locals);
  Object *result = Machine_run(machine, MainObject);
  Object_destroy(result);
  Machine_destroy(machine);
}

int main(int argc, char const *argv[])
{
  long literals[] = {
    (long) 30,
    (long) "this is a string test, pretty long by the way. do you think it will fit in a (long)?",
    (long) "print",
    (long) "add"
  };

  uint32_t encoded_instructions[] = {
    OP_LOADI(1, 0),
    OP_LOADI(2, 0),
    OP_ADD(0, 1, 2),
    OP_MOVE(4, 0),
    OP_DUMP(),
    OP_RET(),
  };

  Instruction instructions[20];

  int i = 0;
  int length = sizeof(encoded_instructions) / sizeof(uint32_t);
  for(i=0; i < length; i++) {
    instructions[i] = Instruction_new(encoded_instructions[i]);
  }

  init_runtime();
  run(literals, instructions);
  destroy_runtime();

  return 0;
}
