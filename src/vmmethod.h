#ifndef __vmmethod_h__
#define __vmmethod_h__

#include "opcode.h"
#include "instruction.h"
// #include "object.h"

typedef struct {
  Instruction **instructions;
  long *literals;
  short arity;
} VMMethod;

VMMethod* VMMethod_new(Instruction **instructions, long *literals, short arity);
// Object* VMMethod_execute(VMMethod *method, Object **locals, Object *self);
void VMMethod_destroy(VMMethod *vmmethod);

Instruction* allocate_instructions(Instruction *instructions, int count);
long* allocate_literals(long *literals, int count);

#endif
