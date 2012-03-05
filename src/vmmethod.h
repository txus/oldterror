#ifndef __vmmethod_h__
#define __vmmethod_h__

#include "opcode.h"
#include "instruction.h"
// #include "object.h"

typedef struct {
  Instruction *start_ip;
  long *literals;
} VMMethod;

VMMethod* VMMethod_new(Instruction *start_ip, long *literals);
// Object* VMMethod_execute(VMMethod *method, Object **locals, Object *self);
void VMMethod_destroy(VMMethod *vmmethod);

Instruction* allocate_instructions(Instruction *instructions, int count);
long* allocate_literals(long *literals, int count);

#endif
