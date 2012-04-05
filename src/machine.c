#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "instruction.h"
#include "registers.h"
#include "object.h"
#include "runtime.h"
#include "dbg.h"
#include "util.h"
#include "gc.h"

Object *TrueObject;
Object *FalseObject;
Object *NilObject;
Object *MainObject;

Machine* Machine_new(Instruction **instructions, long *literals, Object **locals) {
  Machine *machine = malloc(sizeof(Machine));
  machine->instructions = instructions;
  machine->literals     = literals;
  machine->locals       = locals;
  return machine;
}

void Machine_destroy(Machine *machine) {
  free(machine);
}

Object* Machine_run(Machine *machine, Object *self) {
  Instruction *ip = machine->instructions[0];
  long *literals  = machine->literals;
  Object **locals = machine->locals;

  Object *regs[NUM_REGISTERS] = { NilObject };   // the registers

  // Main VM loop
  while(1) {
    switch(ip->opcode) {
      case MOVE: {
        debug("MOVE %i %i", ip->fields.a, ip->fields.b);
        REGISTER(regs[ip->fields.a], regs[ip->fields.b]);
        break;
      }
      case LOADI: {
        debug("LOADI %i %i", ip->fields.a, ip->fields.b);

        //CLEAN_REGISTER(regs[ip->fields.a]);
        REGISTER(regs[ip->fields.a], Integer_new((int)literals[ip->fields.b]));
        break;
      }
      case LOADS: {
        debug("LOADS %i %i", ip->fields.a, ip->fields.b);
        //CLEAN_REGISTER(regs[ip->fields.a]);
        REGISTER(regs[ip->fields.a], String_new((const char *)literals[ip->fields.b]));
        break;
      }
      case LOADBOOL: {
        debug("LOADBOOL %i %i", ip->fields.a, ip->fields.b);
        //CLEAN_REGISTER(regs[ip->fields.a]);
        if (ip->fields.b == 0)
        {
          regs[ip->fields.a] = FalseObject;
        } else {
          regs[ip->fields.a] = TrueObject;
        }
        break;
      }
      case LOADNIL: {
        debug("LOADNIL %i", ip->fields.a);
        //CLEAN_REGISTER(regs[ip->fields.a]);
        regs[ip->fields.a] = NilObject;
        break;
      }
      case ADD: {
        debug("ADD %i %i %i", ip->fields.a, ip->fields.b, ip->fields.c);
        //CLEAN_REGISTER(regs[ip->fields.a]);

        Object *left  = regs[ip->fields.b];
        Object *right = regs[ip->fields.c];

        if(left->type != tInteger || right->type != tInteger) {
          printf("Cannot ADD two non-integers.\n");
          printf("\t- ");
          Object_print(left);
          printf("\n\t- ");
          Object_print(right);
          printf("\n");
          exit(1);
        }
        // Add the two integer values
        int result = left->value.integer + right->value.integer;

        REGISTER(regs[ip->fields.a], Integer_new(result));
        break;
      }
      case SUB: {
        debug("SUB %i %i %i", ip->fields.a, ip->fields.b, ip->fields.c);

        //CLEAN_REGISTER(regs[ip->fields.a]);

        Object *left  = regs[ip->fields.b];
        Object *right = regs[ip->fields.c];

        if(left->type != tInteger || right->type != tInteger) {
          printf("Cannot SUB two non-integers.\n");
          printf("\t- ");
          Object_print(left);
          printf("\n\t- ");
          Object_print(right);
          printf("\n");
          exit(1);
        }
        // Subtract the two integer values
        int result = left->value.integer - right->value.integer;

        REGISTER(regs[ip->fields.a], Integer_new(result));
        break;
      }
      case MUL: {
        debug("MUL %i %i %i", ip->fields.a, ip->fields.b, ip->fields.c);

        //CLEAN_REGISTER(regs[ip->fields.a]);

        Object *left  = regs[ip->fields.b];
        Object *right = regs[ip->fields.c];

        if(left->type != tInteger || right->type != tInteger) {
          printf("Cannot MUL two non-integers.\n");
          printf("\t- ");
          Object_print(left);
          printf("\n\t- ");
          Object_print(right);
          printf("\n");
          exit(1);
        }
        // Subtract the two integer values
        int result = left->value.integer * right->value.integer;

        REGISTER(regs[ip->fields.a], Integer_new(result));
        break;
      }
      case DIV: {
        debug("DIV %i %i %i", ip->fields.a, ip->fields.b, ip->fields.c);

        //CLEAN_REGISTER(regs[ip->fields.a]);

        Object *left  = regs[ip->fields.b];
        Object *right = regs[ip->fields.c];

        if(left->type != tInteger || right->type != tInteger) {
          printf("Cannot MUL two non-integers.\n");
          printf("\t- ");
          Object_print(left);
          printf("\n\t- ");
          Object_print(right);
          printf("\n");
          exit(1);
        }
        // Subtract the two integer values
        int result = left->value.integer / right->value.integer;

        REGISTER(regs[ip->fields.a], Integer_new(result));
        break;
      }
      case JMP: {
        debug("JMP %i", ip->fields.a);
        ip = ip + (ip->fields.a - 1); // Jump N instructions
        break;
      }
      case JIF: {
        debug("JIF %i %i", ip->fields.a, ip->fields.b);

        Object *value = regs[ip->fields.b];
        if (value == FalseObject || value == NilObject) {
          //ip = ip + (ip->fields.a - 1); // Jump N instructions
          ip += sizeof(Instruction) * (ip->fields.a - 1);
        }

        break;
      }
      case JIT: {
        debug("JIT %i %i", ip->fields.a, ip->fields.b);

        Object *value = regs[ip->fields.b];
        if (value != FalseObject && value != NilObject) {
          //ip = ip + (ip->fields.a - 1); // Jump N instructions
          ip += sizeof(Instruction) * (ip->fields.a - 1);
        }

        break;
      }
      case LOADSELF: {
        debug("LOADSELF %i", ip->fields.a);
        //CLEAN_REGISTER(regs[ip->fields.a]);
        REGISTER(regs[ip->fields.a], self);
        break;
      }
      case LOADLOCAL: {
        debug("LOADLOCAL %i %i", ip->fields.a, ip->fields.b);
        //CLEAN_REGISTER(regs[ip->fields.a]);
        REGISTER(regs[ip->fields.a], locals[ip->fields.b]);
        break;
      }
      case SETLOCAL: {
        debug("SETLOCAL %i %i", ip->fields.a, ip->fields.b);
        locals[ip->fields.b] = regs[ip->fields.a];
        break;
      }
      case SEND: {
        Object   *receiver  = regs[ip->fields.a];
        const char *message = (const char *)(regs[ip->fields.b]->value.string);
        VMMethod *method    = Object_lookup_method(receiver, message);

        if (!method) {
          printf("Cannot find method %s on ", message);
          Object_print(receiver);
          printf("\n");
          die("Method lookup error.");
        }

        Object *argv[method->arity - 1];
        int i = 0;

        // From the start register of the arguments (C),
        // get the arguments R(C+0), R(C+1) ... R(C+ARITY)
        for(i=0; i<method->arity; i++) {
          argv[i] = regs[ip->fields.c + i];
        }

        REGISTER(regs[ip->fields.a],
            call(receiver, message, argv, method->arity));

        release(receiver);

        break;
      }
      case RET: {
        debug("RET %i", ip->fields.a);
        Registers_cleanup(regs);
        return regs[ip->fields.a];
        break;
      }
      case DUMP: {
        debug("DUMP");
        Registers_print(regs);
        break;
      }
    }
    ip += sizeof(Instruction);
  }
}
