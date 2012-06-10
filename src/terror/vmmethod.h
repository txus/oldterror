#ifndef _terror__vmmethod_h_
#define _terror__vmmethod_h_

#include <terror/opcode.h>
#include <terror/instruction.h>
// #include <terror/object.h>

typedef struct {
  Instruction **instructions;
  long *literals;

  int instructions_count;
  int literals_count;

  short arity;
} VMMethod;

VMMethod* VMMethod_new(Instruction **instructions, int instructions_count, long *literals, int literals_count, short arity);
void VMMethod_destroy(VMMethod *vmmethod);
#endif
