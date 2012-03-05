#include <string.h>
#include "clay.h"
#include "../src/dbg.h"
#include "../src/machine.h"
#include "../src/runtime.h"

static VMMethod* test_create_method(Instruction instructions[], int count)
{
  long literals[] = {
    (long) "foo",
    (long) 30,
    (long) "add",
  };

  int literals_count = sizeof(literals) / sizeof(long);
  long *allocated_literals     = allocate_literals(literals, literals_count);
  Instruction *allocated_instructions = allocate_instructions(instructions, count);

  VMMethod *method = VMMethod_new(allocated_instructions, allocated_literals);
  return method;
}

static Machine* test_create_machine(Instruction instructions[], int count)
{
  VMMethod *method = test_create_method(instructions, count);

  Object **locals = malloc(sizeof(Object) * 1);
  locals[0] = Integer_new(123); // Set a local variable

  Machine *machine = Machine_new(method->start_ip, method->literals, locals);

  return machine;
}

static Object* test_run_instructions(Instruction instructions[], int count)
{
  Machine *machine = test_create_machine(instructions, count);
  Object *self = Object_new();
  Object *result = Machine_run(machine, self);

  /* Object_destroy(self); */
  Machine_destroy(machine);
  return result;
}

void test_machine_test__new(void)
{
  Instruction instructions[] = {
    Instruction_new(0x01000100), // LOADI 0 1
    Instruction_new(0x90000000), // RET
  };

  Machine *machine = test_create_machine(instructions, 2);
  cl_assert(machine->ip->opcode == LOADI);

  Machine_destroy(machine);
}

/*
 * TEST INSTRUCTIONS
 */

void test_machine_test__run_push_int(void)
{
  Instruction instructions[] = {
    Instruction_new(0x01000100), // LOADI 0 1
    Instruction_new(0x90000000), // RET
  };

  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 30);

  Object_destroy(result);
}

void test_machine_test__run_load_string(void)
{
  Instruction instructions[] = {
    Instruction_new(0x02000000), // LOADS 0 0
    Instruction_new(0x90000000), // RET
  };

  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tString);
  cl_assert(strcmp(result->value.string, "foo") == 0);
}

void test_machine_test__run_load_false(void)
{
  Instruction instructions[] = {
    Instruction_new(0x09000000), // LOADBOOL 0 0
    Instruction_new(0x90000000), // RET
  };

  init_runtime(); //needed for FalseObject and TrueObject
  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tFalse);

  destroy_runtime();
}

void test_machine_test__run_load_true(void)
{
  Instruction instructions[] = {
    Instruction_new(0x09000100), // LOADBOOL 0 1
    Instruction_new(0x90000000), // RET
  };

  init_runtime(); //needed for FalseObject and TrueObject
  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tTrue);

  destroy_runtime();
}

void test_machine_test__run_load_nil(void)
{
  Instruction instructions[] = {
    Instruction_new(0x08000000), // LOADNIL 0
    Instruction_new(0x90000000), // RET
  };

  init_runtime(); //needed for FalseObject and TrueObject
  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tNil);

  destroy_runtime();
}

void test_machine_test__run_load_local(void)
{
  Instruction instructions[] = {
    Instruction_new(0x50000000), // LOAD_LOCAL 0 0
    Instruction_new(0x90000000), // RET
  };

  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 123);

  Object_destroy(result);
}

void test_machine_test__run_set_local(void)
{
  Instruction instructions[] = {
    Instruction_new(0x01010100), // LOADI      1 1
    Instruction_new(0x51010100), // SET_LOCAL  1 1
    Instruction_new(0x50000100), // LOAD_LOCAL 0 1
    Instruction_new(0x90000000), // RET
  };

  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 30);

  Object_destroy(result);
}


void test_machine_test__run_load_self(void)
{
  Instruction instructions[] = {
    Instruction_new(0x30000000), // LOADSELF 0
    Instruction_new(0x90000000), // RET
  };

  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tObject);

  Object_destroy(result);
}

void test_machine_test__run_add(void)
{
  Instruction instructions[] = {
    Instruction_new(0x01010100), // LOADI 1 1
    Instruction_new(0x01020100), // LOADI 2 1
    Instruction_new(0x10000102), // ADD   0 1 2
    Instruction_new(0x90000000)  // RET
  };

  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 60);

  Object_destroy(result);
}

void test_machine_test__run_sub(void)
{
  Instruction instructions[] = {
    Instruction_new(0x01010100), // LOADI 1 1
    Instruction_new(0x01020100), // LOADI 2 1
    Instruction_new(0x11000102), // SUB   0 1 2
    Instruction_new(0x90000000)  // RET
  };

  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 0);

  Object_destroy(result);
}

void test_machine_test__run_mul(void)
{
  Instruction instructions[] = {
    Instruction_new(0x01010100), // LOADI 1 1
    Instruction_new(0x01020100), // LOADI 2 1
    Instruction_new(0x12000102), // MUL   0 1 2
    Instruction_new(0x90000000)  // RET
  };

  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 900);

  Object_destroy(result);
}

void test_machine_test__run_div(void)
{
  Instruction instructions[] = {
    Instruction_new(0x01010100), // LOADI 1 1
    Instruction_new(0x01020100), // LOADI 2 1
    Instruction_new(0x13000102), // DIV   0 1 2
    Instruction_new(0x90000000)  // RET
  };

  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 1);

  Object_destroy(result);
}

/* void test_machine_test__run_jump_unless(void) */
/* { */
/*   Instruction dont_jump_instructions[] = { */
/*     PUSH_INT, 1, */
/*     PUSH_BOOL, 1, */
/*     JUMP_UNLESS, 2, */
/*     PUSH_STRING, 0, */
/*     RET, */
/*   }; */
/*  */
/*   Instruction jump_instructions[] = { */
/*     PUSH_INT, 1, */
/*     PUSH_BOOL, 0, */
/*     JUMP_UNLESS, 2, */
/*     PUSH_STRING, 0, */
/*     RET, */
/*   }; */
/*  */
/*   init_runtime(); */
/*  */
/*   Object *dont_jump_result = test_run_instructions(dont_jump_instructions, 9); */
/*  */
/*   cl_assert(dont_jump_result); */
/*   cl_assert(dont_jump_result->type == tString); */
/*   cl_assert(strcmp(dont_jump_result->value.string, "foo") == 0); */
/*  */
/*   Object *jump_result = test_run_instructions(jump_instructions, 9); */
/*  */
/*   cl_assert(jump_result); */
/*   cl_assert(jump_result->type == tInteger); */
/*   cl_assert(jump_result->value.integer == 30); */
/*  */
/*   Object_destroy(dont_jump_result); */
/*   Object_destroy(jump_result); */
/* } */

/* void test_machine_test__call(void) */
/* { */
/*   Instruction instructions[] = { */
/*     PUSH_INT, 1, */
/*     PUSH_INT, 1, */
/*     CALL, 2, 1, */
/*     RET, */
/*   }; */
/*  */
/*   Object *result = test_run_instructions(instructions, 8); */
/*  */
/*   cl_assert(result); */
/*   cl_assert(result->type == tInteger); */
/*   cl_assert(result->value.integer == 60); */
/*   cl_assert(result->refcount == 1); */
/*  */
/*   Object_destroy(result); */
/* } */
