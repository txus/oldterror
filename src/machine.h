#ifndef __machine_h__
#define __machine_h__

#include "opcode.h"
#include "instruction.h"
#include "object.h"

typedef struct {
  Instruction *ip;
  long *literals;
  Object **locals;
} Machine;

Machine* Machine_new(Instruction *ip, long *literals, Object **locals);
void Machine_destroy(Machine *machine);
Object* Machine_run(Machine *machine, Object *self);

#endif
