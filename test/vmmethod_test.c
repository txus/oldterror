#include <stdint.h>
#include "clay.h"
#include "../src/vmmethod.h"
#include "../src/instruction.h"
#include "../src/object.h"

Object* VMMethod_execute(VMMethod *method, Object **locals, Object *self);

static VMMethod *test_create_method()
{
  long literals[] = {
    (long) "foo",
    (long) 30,
  };

  Instruction instructions[] = {
    Instruction_new(0x02020000), // LOADS 2 0
    Instruction_new(0x30010000), // LOADSELF 1
    Instruction_new(0x01000100), // LOADI 0 1
    Instruction_new(0x90000000), // RET
  };

  int count = (sizeof(instructions) / sizeof(Instruction)) + 1;
  Instruction *allocated_instructions = allocate_instructions(instructions, count);

  VMMethod *method = VMMethod_new(allocated_instructions, allocate_literals(literals, 2));
  return method;
}

void test_vmmethod_test__new(void)
{
  VMMethod *method = test_create_method();

  uint8_t opcode = method->start_ip->opcode;
  long first_literal = method->literals[0];

  cl_must_pass(method);
  cl_must_pass(opcode == LOADS);
  cl_must_pass(first_literal == (long) "foo");

  VMMethod_destroy(method);
}

void test_vmmethod_test__execute(void)
{
  Object *self = Main_new();
  Object *locals[10];

  VMMethod *method = test_create_method();

  Object *result = VMMethod_execute(method, locals, self);

  cl_must_pass(result->value.integer == 30);

  VMMethod_destroy(method);
  Object_destroy(result);
  free(self);
}
