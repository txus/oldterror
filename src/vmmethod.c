#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "vmmethod.h"
#include "registers.h"
#include "machine.h"
#include "util.h"

Object* VMMethod_execute(VMMethod *method, Object **locals, Object *self);

VMMethod* VMMethod_new(Instruction *start_ip, long *literals) {
  VMMethod *method = malloc(sizeof(VMMethod));
  if (!method) die("Could not allocate VMMethod");
  method->start_ip = start_ip;
  method->literals = literals;
  return method;
}

void VMMethod_destroy(VMMethod *method) {
  free(method->start_ip);
  free(method->literals);
  free(method);
}

Object* VMMethod_execute(VMMethod *method, Object **locals, Object *self) {
  Machine *machine = Machine_new(method->start_ip, method->literals, locals);
  Object *result = Machine_run(machine, self);
  Machine_destroy(machine);
  return result;
}

Instruction* allocate_instructions(Instruction *instructions, int count) {
  Instruction *allocated_instructions = malloc(sizeof(Instruction) * count);

  int i = 0;
  for(i = 0; i < count; i++) {
    allocated_instructions[i] = instructions[i];
  }

  return allocated_instructions;
}

long* allocate_literals(long literals[], int count) {
  long *allocated_literals = malloc(sizeof(long) * count);

  int i = 0;
  for(i = 0; i < count; i++) {
    allocated_literals[i] = literals[i];
  }

  return allocated_literals;
}
