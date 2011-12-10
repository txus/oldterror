#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "stack.h"
#include "object.h"
#include "runtime.h"
#include "dbg.h"
#include "gc.h"

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

Object* Machine_run(Machine *machine, Object *self) {
  byte *ip        = machine->ip;
  long *literals  = machine->literals;
  Object **locals = machine->locals;

  Object *stack[STACK_MAX];   // the Data stack
  Object **sp = stack; // the stack pointer

  // Main VM loop
  while(1) {
    switch(*ip) {
      // PUSH literals
      case PUSH_INT:
        ip++; // Moving to the operand
        debug("PUSH_INT %i", *ip);
        STACK_PUSH(Integer_new((int)literals[*ip]));
        break;
      case PUSH_STRING:
        ip++; // Moving to the operand
        debug("PUSH_STRING %i", *ip);
        STACK_PUSH(String_new((char *)literals[*ip]));
        break;
      case PUSH_BOOL:
        ip++; // 0=false, 1=true
        debug("PUSH_BOOL %i", *ip);
        if (*ip == 0) {
          STACK_PUSH(FalseObject);
        } else {
          STACK_PUSH(TrueObject);
        }
        break;
      case PUSH_NIL:
        debug("PUSH_NIL");
        STACK_PUSH(NilObject);
        break;

      case CALL: {
        ip++; // advance to index of method name in literal table
        char *method = (char *)literals[*ip];
        ip++; // number of arguments
        int argc = *ip;
        Object *argv[10];

        debug("CALL %i %i", *(ip-1), *(ip));

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
        debug("GET_LOCAL %i", *ip);
        STACK_PUSH(locals[*ip]);
        break;
      case SET_LOCAL:
        ip++;
        debug("SET_LOCAL %i", *ip);
        locals[*ip] = STACK_PEEK();
        break;

      // POP
      case POP: {
        debug("POP");
        Object *popped = STACK_POP();
        free(popped);
        break;
      }

      // PUSH the self object
      case PUSH_SELF:
        debug("PUSH_SELF");
        STACK_PUSH(self);
        break;

      // Arithmetic
      case ADD: {
        debug("ADD");
        Object *a = STACK_POP();
        Object *b = STACK_POP();

        if(a->type != tInteger || b->type != tInteger) {
          printf("Cannot ADD two non-integers.\n");
          printf("\t- ");
          Object_print(a);
          printf("\n\t- ");
          Object_print(b);
          printf("\n");
          exit(1);
        }
        // Add the two integer values
        int result = a->value.integer + b->value.integer;

        STACK_PUSH(Integer_new(result));
        break;
      }
      case SUB: {
        debug("SUB");
        Object *a = STACK_POP();
        Object *b = STACK_POP();

        // Subtract the two integer values
        int result = a->value.integer - b->value.integer;

        STACK_PUSH(Integer_new(result));
        break;
      }
      case MUL: {
        debug("MUL");
        Object *a = STACK_POP();
        Object *b = STACK_POP();

        // Multiply the two integer values
        int result = a->value.integer * b->value.integer;

        STACK_PUSH(Integer_new(result));
        break;
      }
      case DIV: {
        debug("DIV");
        Object *a = STACK_POP();
        Object *b = STACK_POP();
        if (b->value.integer == 0) {
          printf("Invalid DIV opcode: dividing by 0\n");
          exit(1);
        }

        // Multiply the two integer values
        int result = a->value.integer / b->value.integer;

        STACK_PUSH(Integer_new(result));
        break;
      }

      // JUMP
      case JUMP_UNLESS: {
        ip++; // number of bytes to move forward
        byte offset = *ip;
        debug("JUMP_UNLESS %i", offset);
        Object *condition = STACK_POP();

        if (!Object_is_true(condition)) ip += offset;

        break;
      }

      // Other
      case DEBUG: {
        debug("DEBUG");
        Stack_print(stack, sp);
        break;
      }
      case DEBUG_TOS: {
        debug("DEBUG_TOS");
        printf("Top of the Stack: ");
        Object_print(*sp);
        printf("\n");
        break;
      }
      case RET: {
        debug("RET");
        return *sp;
      }
    }
    ip++;
  }
}
