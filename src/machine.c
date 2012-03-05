#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "instruction.h"
#include "registers.h"
#include "object.h"
#include "runtime.h"
#include "dbg.h"
#include "gc.h"

Object *TrueObject;
Object *FalseObject;
Object *NilObject;
Object *MainObject;

Machine* Machine_new(Instruction *ip, long *literals, Object **locals) {
  Machine *machine = malloc(sizeof(Machine));
  machine->ip           = ip;
  machine->literals     = literals;
  machine->locals       = locals;
  return machine;
}

void Machine_destroy(Machine *machine) {
  /* free(machine->locals); */
  free(machine);
}

Object* Machine_run(Machine *machine, Object *self) {
  Instruction *ip = machine->ip;
  long *literals  = machine->literals;
  /* Object **locals = machine->locals; */

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
        REGISTER(regs[ip->fields.a], Integer_new((int)literals[ip->fields.b]));
        break;
      }
      case ADD: {
        debug("ADD %i %i %i", ip->fields.a, ip->fields.b, ip->fields.c);
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

        /* release(left); */
        /* release(right); */

        REGISTER(regs[ip->fields.a], Integer_new(result));
        break;
      }
      case SUB: {
        debug("SUB");
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

        release(left);
        release(right);

        REGISTER(regs[ip->fields.a], Integer_new(result));
        break;
      }
      case DUMP: {
        debug("DUMP");
        Registers_print(regs);
        break;
      }
      case RET: {
        debug("RET");
        Registers_cleanup(regs);
        return regs[0];
        break;
      }
    }
    ip++;
  }
}
