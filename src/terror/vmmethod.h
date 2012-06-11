#ifndef _terror__vmmethod_h_
#define _terror__vmmethod_h_

#include <terror/opcode.h>
#include <terror/instruction.h>
#include <terror/object.h>

struct vmmethod_s {
  Instruction **instructions;
  long *literals;

  int instructions_count;
  int literals_count;

  short arity;
};

typedef struct vmmethod_s VMMethod;

VMMethod* VMMethod_new(Instruction **instructions, int instructions_count, long *literals, int literals_count, short arity);
void VMMethod_destroy(VMMethod *vmmethod);
Object* VMMethod_execute(VMMethod *method, Object **locals, int locals_count, int registers_count, Object *self);
#endif
