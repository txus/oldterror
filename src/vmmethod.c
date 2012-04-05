#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "vmmethod.h"
#include "registers.h"
#include "machine.h"
#include "util.h"

Object* VMMethod_execute(VMMethod *method, Object **locals, Object *self);

VMMethod* VMMethod_new(Instruction **instructions, long *literals, short arity) {
  VMMethod *method = malloc(sizeof(VMMethod));
  if (!method) die("Could not allocate VMMethod");
  method->instructions = instructions;
  method->literals = literals;
  method->arity    = arity;
  return method;
}

void VMMethod_destroy(VMMethod *method) {
  free(method->instructions);
  free(method->literals);
  free(method);
}

Object* VMMethod_execute(VMMethod *method, Object **locals, Object *self) {
  Machine *machine = Machine_new(method->instructions, method->literals, locals);
  Object *result = Machine_run(machine, self);
  Machine_destroy(machine);
  return result;
}
