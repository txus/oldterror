#include <stdio.h>
#include "dbg.h"
#include "opcode.h"
#include "object.h"
#include "stack.h"
#include "runtime.h"
#include "machine.h"

// Extern global objects declared in runtime.h
Object *TrueObject;
Object *FalseObject;
Object *NilObject;
Object *MainObject;

void run(long literals[], byte instructions[]) {
  // Instruction pointer
  byte *ip = instructions;
  Object *locals[STACK_MAX] = {};

  Machine *machine = Machine_new(ip, literals, locals);
  Machine_run(machine, MainObject);
}

int main(int argc, char const *argv[])
{
  long literals[] = {
    (long) 30,
    (long) "this is a string test, pretty long by the way. do you think it will fit in a (long)?",
    (long) "print",
    (long) "add"
  };

  byte instructions[] = {
    PUSH_INT, 0,
    PUSH_INT, 0,
    CALL, 3, 1,
    DEBUG_TOS,
    RET
  };

  init_runtime();
  run(literals, instructions);
  destroy_runtime();

  return 0;
}
