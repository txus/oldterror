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

byte* allocate_instructions(byte *instructions) {
  int count = sizeof(instructions) / sizeof(byte);
  byte *allocated_instructions = malloc(sizeof(byte) * count);

  int i = 0;
  for(i = 0; i < count; i++) {
    allocated_instructions[i] = instructions[i];
  }

  return allocated_instructions;
}
