#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "vmmethod.h"
#include "stack.h"
#include "machine.h"

VMMethod* VMMethod_new(byte *start_ip, long *literals) {
  VMMethod *method = malloc(sizeof(VMMethod));
  method->start_ip = start_ip;
  method->literals = literals;
  return method;
}

Object* VMMethod_execute(VMMethod *method, Object **locals, Object *self) {
  Machine *machine = Machine_new(method->start_ip, method->literals, locals);
  Object *result = Machine_run(machine, self);
  return result;
}
