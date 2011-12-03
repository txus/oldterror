#include <stdio.h>
#include "dbg.h"
#include "opcode.h"
#include "object.h"
#include "stack.h"

void run(long literals[], byte instructions[]) {
  // Instruction pointer
  byte *ip = instructions;

  Object *stack[STACK_MAX];   // the Data stack
  Object **sp = stack; // the stack pointer

  // Initialize the main Runtime object
  Object *self = Object_new();

  // Main VM loop
  while(1) {
    printf("Executing instruction [%i]\n", *ip);
    switch(*ip) {
      case PUSH_INT:
        ip++; // Moving to the operand
        STACK_PUSH(Integer_new((int)literals[*ip]));
        break;
      case PUSH_STRING:
        ip++; // Moving to the operand
        STACK_PUSH(String_new((char *)literals[*ip]));
        break;
      case PUSH_SELF:
        STACK_PUSH(self);
        break;
      case ADD: {
        Object *a = STACK_POP();
        Object *b = STACK_POP();

        // Add the two integer values
        int result = a->value.integer + b->value.integer;

        STACK_PUSH(Integer_new(result));
        break;
      }
      case POP: {
        Object *popped = STACK_POP();
        break;
      }
      case DEBUG: {
        Stack_print(stack, sp);
        break;
      }
      case RET: {
        return;
      }
    }
    ip++;
  }
}

int main(int argc, char const *argv[])
{
  long literals[] = {
    (long) 30,
    (long) "this is a string test, pretty long by the way. do you think it will fit in a (long)?"
  };

  byte instructions[] = {
    PUSH_INT, 0,
    PUSH_INT, 0,
    PUSH_INT, 0,
    PUSH_INT, 0,
    POP,
    POP,
    ADD,
    PUSH_STRING, 1,
    PUSH_SELF,
    DEBUG,
    RET
  };

  run(literals, instructions);

  return 0;
}
