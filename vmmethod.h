#ifndef __vmmethod_h__
#define __vmmethod_h__

#include "opcode.h"

typedef struct {
  byte **start_ip;
} VMMethod;

VMMethod *VMMethod_new(byte **start_ip);
// Object *VMMethod_execute(long literals[], Object *locals[]);

#endif
