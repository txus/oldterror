#include <string.h>
#include "clay.h"
#include "../src/machine.h"
#include "../src/runtime.h"

static VMMethod* test_create_method(byte instructions[], int instructions_count)
{
  long literals[] = {
    (long) "foo",
    (long) 30,
    (long) "add",
  };

  int literals_count = sizeof(literals) / sizeof(long);
  long *allocated_literals     = allocate_literals(literals, literals_count);
  byte *allocated_instructions = allocate_instructions(instructions, instructions_count);

  VMMethod *method = VMMethod_new(allocated_instructions, allocated_literals);
  return method;
}

static Machine* test_create_machine(byte instructions[], int count)
{
  VMMethod *method = test_create_method(instructions, count);

  Object **locals = malloc(sizeof(Object) * 1);
  locals[0] = Integer_new(123); // Set a local variable

  Machine *machine = Machine_new(method->start_ip, method->literals, locals);

  VMMethod_destroy(method);
  return machine;
}

static Object* test_run_instructions(byte instructions[], int count)
{
  Machine *machine = test_create_machine(instructions, count);
  Object *self = Object_new();
  Object *result = Machine_run(machine, self);

  Object_destroy(self);
  Machine_destroy(machine);
  return result;
}

void test_machine_test__new(void)
{
  byte instructions[] = {
    PUSH_INT, 1,
    RET,
  };

  Machine *machine = test_create_machine(instructions, 3);
  cl_assert(*(machine->ip) == PUSH_INT);

  Machine_destroy(machine);
}

/*
 * TEST INSTRUCTIONS
 */

void test_machine_test__run_push_int(void)
{
  byte instructions[] = {
    PUSH_INT, 1,
    RET,
  };

  Object *result = test_run_instructions(instructions, 3);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 30);

  Object_destroy(result);
}

void test_machine_test__run_push_string(void)
{
  byte instructions[] = {
    PUSH_STRING, 0,
    RET,
  };

  Object *result = test_run_instructions(instructions, 3);

  cl_assert(result);
  cl_assert(result->type == tString);
  cl_assert(strcmp(result->value.string, "foo") == 0);
}

void test_machine_test__run_push_false(void)
{
  byte instructions[] = {
    PUSH_BOOL, 0,
    RET,
  };

  init_runtime(); //needed for FalseObject and TrueObject
  Object *result = test_run_instructions(instructions, 3);

  cl_assert(result);
  cl_assert(result->type == tFalse);

  destroy_runtime();
}

void test_machine_test__run_push_true(void)
{
  byte instructions[] = {
    PUSH_BOOL, 1,
    RET,
  };

  init_runtime(); //needed for FalseObject and TrueObject
  Object *result = test_run_instructions(instructions, 3);

  cl_assert(result);
  cl_assert(result->type == tTrue);

  destroy_runtime();
}

void test_machine_test__run_push_nil(void)
{
  byte instructions[] = {
    PUSH_NIL,
    RET,
  };

  init_runtime(); //needed for FalseObject and TrueObject
  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tNil);

  destroy_runtime();
}

void test_machine_test__run_get_local(void)
{
  byte instructions[] = {
    GET_LOCAL, 0,
    RET,
  };

  Object *result = test_run_instructions(instructions, 3);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 123);

  Object_destroy(result);
}

void test_machine_test__run_set_local(void)
{
  byte instructions[] = {
    PUSH_INT, 1,
    SET_LOCAL, 1,
    GET_LOCAL, 1,
    RET,
  };

  Object *result = test_run_instructions(instructions, 7);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 30);

  Object_destroy(result);
}

void test_machine_test__run_pop(void)
{
  byte instructions[] = {
    PUSH_INT, 1,
    PUSH_STRING, 0,
    POP,
    RET,
  };

  Object *result = test_run_instructions(instructions, 6);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 30);

  Object_destroy(result);
}

void test_machine_test__run_push_self(void)
{
  byte instructions[] = {
    PUSH_SELF,
    RET,
  };

  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tObject);

  Object_destroy(result);
}

void test_machine_test__run_add(void)
{
  byte instructions[] = {
    PUSH_INT, 1,
    PUSH_INT, 1,
    ADD,
    RET,
  };

  Object *result = test_run_instructions(instructions, 6);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 60);

  Object_destroy(result);
}

void test_machine_test__run_sub(void)
{
  byte instructions[] = {
    PUSH_INT, 1,
    PUSH_INT, 1,
    SUB,
    RET,
  };

  Object *result = test_run_instructions(instructions, 6);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 0);

  Object_destroy(result);
}

void test_machine_test__run_mul(void)
{
  byte instructions[] = {
    PUSH_INT, 1,
    PUSH_INT, 1,
    MUL,
    RET,
  };

  Object *result = test_run_instructions(instructions, 6);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 900);

  Object_destroy(result);
}

void test_machine_test__run_div(void)
{
  byte instructions[] = {
    PUSH_INT, 1,
    PUSH_INT, 1,
    DIV,
    RET,
  };

  Object *result = test_run_instructions(instructions, 6);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 1);

  Object_destroy(result);
}

void test_machine_test__run_jump_unless(void)
{
  byte dont_jump_instructions[] = {
    PUSH_INT, 1,
    PUSH_BOOL, 1,
    JUMP_UNLESS, 2,
    PUSH_STRING, 0,
    RET,
  };

  byte jump_instructions[] = {
    PUSH_INT, 1,
    PUSH_BOOL, 0,
    JUMP_UNLESS, 2,
    PUSH_STRING, 0,
    RET,
  };

  init_runtime();

  Object *dont_jump_result = test_run_instructions(dont_jump_instructions, 9);

  cl_assert(dont_jump_result);
  cl_assert(dont_jump_result->type == tString);
  cl_assert(strcmp(dont_jump_result->value.string, "foo") == 0);

  Object *jump_result = test_run_instructions(jump_instructions, 9);

  cl_assert(jump_result);
  cl_assert(jump_result->type == tInteger);
  cl_assert(jump_result->value.integer == 30);

  Object_destroy(dont_jump_result);
  Object_destroy(jump_result);
}

void test_machine_test__call(void)
{
  byte instructions[] = {
    PUSH_INT, 1,
    PUSH_INT, 1,
    CALL, 2, 1,
    RET,
  };

  Object *result = test_run_instructions(instructions, 8);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 60);
  cl_assert(result->refcount == 1);

  Object_destroy(result);
}
