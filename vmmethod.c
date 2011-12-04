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
  // Create a stack for the method
  Object *stack[STACK_MAX];   // the Data stack
  Object **sp = stack; // the stack pointer

  // Start a new machine to execute the method.
  Machine *machine = Machine_new(method->start_ip, method->literals, locals);
  Machine_run(machine, sp, self);
  return *sp;
}
