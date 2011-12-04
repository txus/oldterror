#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "stack.h"
#include "object.h"
#include "runtime.h"

Object *TrueObject;
Object *FalseObject;
Object *NilObject;
Object *MainObject;

Machine* Machine_new(byte *ip, long *literals, Object **locals) {
  Machine *machine = malloc(sizeof(Machine));
  machine->ip           = ip;
  machine->literals     = literals;
  machine->locals       = locals;
  return machine;
}

void Machine_run(Machine *machine, Object **sp, Object *self) {
  byte *ip        = machine->ip;
  long *literals  = machine->literals;
  Object **locals = machine->locals;

  Object **stack  = sp;

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
