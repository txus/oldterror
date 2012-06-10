#ifndef _terror__machine_h_
#define _terror__machine_h_

#include <terror/opcode.h>
#include <terror/instruction.h>
#include <terror/object.h>

typedef struct {
  Instruction **instructions;
  long *literals;
  Object **locals;

  int instructions_count;
  int literals_count;
  int locals_count;
  int registers_count;
} Machine;

Machine* Machine_new(Instruction **instructions, int instructions_count, long *literals, int literals_count, Object **locals, int locals_count, int registers_count);
Object* Machine_run(Machine *machine, Object *self);

#define Machine_destroy(M) free(M)

#endif
