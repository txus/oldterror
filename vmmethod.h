#ifndef __vmmethod_h__
#define __vmmethod_h__

#include "opcode.h"
#include "object.h"

typedef struct {
  byte *start_ip;
  long *literals;
} VMMethod;

VMMethod* VMMethod_new(byte *start_ip, long *literals);
// Object* VMMethod_execute(VMMethod *method, Object **locals, Object *self);

#endif
