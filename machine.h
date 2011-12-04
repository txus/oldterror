#ifndef __machine_h__
#define __machine_h__

#include "opcode.h"
#include "object.h"

typedef struct {
  byte *ip;
  long *literals;
  Object **locals;
} Machine;

Machine* Machine_new(byte *ip, long *literals, Object **locals);
void Machine_run(Machine *machine, Object **sp, Object *self);

#endif
