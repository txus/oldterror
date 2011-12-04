#include <stdio.h>
#include "dbg.h"
#include "opcode.h"
#include "object.h"
#include "stack.h"
#include "runtime.h"

// Extern global objects declared in runtime.h
Object *TrueObject;
Object *FalseObject;
Object *NilObject;
Object *MainObject;

void run(long literals[], byte instructions[]) {
  // Instruction pointer
  byte *ip = instructions;

  Object *stack[STACK_MAX];   // the Data stack
  Object **sp = stack; // the stack pointer

  Object *locals[STACK_MAX] = {};

  // Initialize the main Runtime object
  Object *self = MainObject;

  // Main VM loop
  while(1) {
    printf("Executing instruction [%i]\n", *ip);
    switch(*ip) {
      // PUSH literals
      case PUSH_INT:
        ip++; // Moving to the operand
        STACK_PUSH(Integer_new((int)literals[*ip]));
        break;
      case PUSH_STRING:
        ip++; // Moving to the operand
        STACK_PUSH(String_new((char *)literals[*ip]));
        break;
      case PUSH_BOOL:
        ip++; // 0=false, 1=true
        if (*ip == 0) {
          STACK_PUSH(FalseObject);
        } else {
          STACK_PUSH(TrueObject);
        }
        break;
      case PUSH_NIL:
        STACK_PUSH(NilObject);
        break;

      case CALL: {
        ip++; // advance to index of method name in literal table
        char *method = (char *)literals[*ip];
        ip++; // number of arguments
        int argc = *ip;
        Object *argv[10];

        // Pop all the arguments
        int i;
        for(i = 0; i < argc; ++i) argv[i] = STACK_POP();

        Object *receiver = STACK_POP();

        Object *result = call(receiver, method, argv, argc);
        STACK_PUSH(result);

        break;
      }

      // Local variables
      case GET_LOCAL:
        ip++;
        STACK_PUSH(locals[*ip]);
        break;
      case SET_LOCAL:
        ip++;
        locals[*ip] = STACK_PEEK();
        break;

      // POP
      case POP: {
        Object *popped = STACK_POP();
        break;
      }

      // PUSH the self object
      case PUSH_SELF:
        STACK_PUSH(self);
        break;

      // Arithmetic
      case ADD: {
        Object *a = STACK_POP();
        Object *b = STACK_POP();

        // Add the two integer values
        int result = a->value.integer + b->value.integer;

        STACK_PUSH(Integer_new(result));
        break;
      }

      // Other
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
    (long) "this is a string test, pretty long by the way. do you think it will fit in a (long)?",
    (long) "print"
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
    SET_LOCAL, 0,
    PUSH_SELF,
    GET_LOCAL, 0,
    CALL, 2, 1,
    DEBUG,
    RET
  };

  init_runtime();
  run(literals, instructions);
  destroy_runtime();

  return 0;
}
