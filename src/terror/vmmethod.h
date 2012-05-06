#ifndef _terror__vmmethod_h_
#define _terror__vmmethod_h_

#include <terror/opcode.h>
#include <terror/instruction.h>
#include <terror/object.h>

typedef struct {
  Instruction **instructions;
  long *literals;
  short arity;
} VMMethod;

VMMethod* VMMethod_new(Instruction **instructions, long *literals, short arity);
void VMMethod_destroy(VMMethod *vmmethod);

#endif
