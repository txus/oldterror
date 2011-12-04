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

  Object *stack[STACK_MAX];   // the Data stack
  Object **sp = stack; // the stack pointer

  Machine *machine = Machine_new(ip, literals, locals);
  Machine_run(machine, sp, MainObject);
}

int main(int argc, char const *argv[])
{
  long literals[] = {
    (long) 30,
    (long) "this is a string test, pretty long by the way. do you think it will fit in a (long)?",
    (long) "print"
  };

  byte instructions[] = {
    PUSH_SELF,
    PUSH_STRING, 1,
    CALL, 2, 1,
    DEBUG,
    RET,
  };

  init_runtime();
  run(literals, instructions);
  destroy_runtime();

  return 0;
}
