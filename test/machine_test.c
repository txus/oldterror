#include <string.h>
#include "clay.h"
#include "../src/dbg.h"
#include "../src/machine.h"
#include "../src/runtime.h"

static VMMethod* test_create_method(Instruction *instructions[], int count)
{
  long literals[] = {
    (long) "foo",
    (long) 30,
    (long) "add",
  };

  VMMethod *method = VMMethod_new(instructions, literals, 0);
  return method;
}

static Machine* test_create_machine(Instruction *instructions[], int count)
{
  VMMethod *method = test_create_method(instructions, count);

  Object **locals = malloc(sizeof(Object) * 1);
  locals[0] = Integer_new(123); // Set a local variable

  Machine *machine = Machine_new(method->instructions, method->literals, locals);

  return machine;
}

static Object* test_run_instructions(Instruction *instructions[], int count)
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
  Instruction *instructions[] = {
    Instruction_new(OP_LOADI(0,1)),
    Instruction_new(OP_RET(0)),
  };

  Machine *machine = test_create_machine(instructions, 2);
  cl_assert(machine->instructions[0]->opcode == LOADI);

  Machine_destroy(machine);
}

/*
 * TEST INSTRUCTIONS
 */

void test_machine_test__run_load_int(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADI(0,1)),
    Instruction_new(OP_RET(0)),
  };

  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  printf("\n\nValue is %i.\n\n", result->value.integer);
  cl_assert(result->value.integer == 30);

  Object_destroy(result);
}

void test_machine_test__run_load_string(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADS(0,0)),
    Instruction_new(OP_RET(0)),
  };

  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tString);
  cl_assert(strcmp(result->value.string, "foo") == 0);
}

void test_machine_test__run_load_false(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADBOOL(0,0)),
    Instruction_new(OP_RET(0)),
  };

  init_runtime(); //needed for FalseObject and TrueObject
  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tFalse);

  destroy_runtime();
}

void test_machine_test__run_load_true(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADBOOL(0,1)),
    Instruction_new(OP_RET(0)),
  };

  init_runtime(); //needed for FalseObject and TrueObject
  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tTrue);

  destroy_runtime();
}

void test_machine_test__run_load_nil(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADNIL(0)),
    Instruction_new(OP_RET(0)),
  };

  init_runtime(); //needed for FalseObject and TrueObject
  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tNil);

  destroy_runtime();
}

void test_machine_test__run_loadlocal(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADLOCAL(0,0)),
    Instruction_new(OP_RET(0)),
  };

  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 123);

  Object_destroy(result);
}

void test_machine_test__run_setlocal(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADI(1,1)),
    Instruction_new(OP_SETLOCAL(1,1)),
    Instruction_new(OP_LOADLOCAL(0,1)),
    Instruction_new(OP_RET(0)),
  };

  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 30);

  Object_destroy(result);
}

void test_machine_test__run_jmp(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADI(0, 1)),
    Instruction_new(OP_JMP(3)),
    Instruction_new(OP_LOADSELF(0)),
    Instruction_new(OP_LOADSELF(0)),
    Instruction_new(OP_RET(0)),
  };

  Object *result = test_run_instructions(instructions, 5);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 30);

  Object_destroy(result);
}

void test_machine_test__run_jif_when_its_truthy_does_nothing(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADI(0, 1)),
    Instruction_new(OP_JIF(2, 0)),
    Instruction_new(OP_LOADSELF(0)),
    Instruction_new(OP_RET(0)),
  };

  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tObject);

  Object_destroy(result);
}

void test_machine_test__run_jif_when_its_falsy_jumps(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADBOOL(0, 0)),
    Instruction_new(OP_JIF(2, 0)),
    Instruction_new(OP_LOADSELF(0)),
    Instruction_new(OP_RET(0)),
  };

  init_runtime(); //needed for FalseObject and TrueObject
  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tFalse);

  Object_destroy(result);
}

void test_machine_test__run_jit_when_its_truthy_jumps(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADI(0, 1)),
    Instruction_new(OP_JIT(2, 0)),
    Instruction_new(OP_LOADSELF(0)),
    Instruction_new(OP_RET(0)),
  };

  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 30);

  Object_destroy(result);
}

void test_machine_test__run_jit_when_its_falsy_does_nothing(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADBOOL(0, 0)),
    Instruction_new(OP_JIT(2, 0)),
    Instruction_new(OP_LOADSELF(0)),
    Instruction_new(OP_RET(0)),
  };

  init_runtime(); //needed for FalseObject and TrueObject
  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tObject);

  Object_destroy(result);
}

void test_machine_test__run_loadself(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADSELF(0)),
    Instruction_new(OP_RET(0)),
  };

  Object *result = test_run_instructions(instructions, 2);

  cl_assert(result);
  cl_assert(result->type == tObject);

  Object_destroy(result);
}

void test_machine_test__run_add(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADI(1,1)),
    Instruction_new(OP_LOADI(2,1)),
    Instruction_new(OP_ADD(0,1,2)),
    Instruction_new(OP_RET(0))
  };

  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 60);

  Object_destroy(result);
}

void test_machine_test__run_sub(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADI(1,1)),
    Instruction_new(OP_LOADI(2,1)),
    Instruction_new(OP_SUB(0,1,2)),
    Instruction_new(OP_RET(0))
  };

  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 0);

  Object_destroy(result);
}

void test_machine_test__run_mul(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADI(1,1)),
    Instruction_new(OP_LOADI(2,1)),
    Instruction_new(OP_MUL(0,1,2)),
    Instruction_new(OP_RET(0))
  };

  Object *result = test_run_instructions(instructions, 4);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 900);

  Object_destroy(result);
}

void test_machine_test__run_div(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADI(1,1)),
    Instruction_new(OP_LOADI(2,1)),
    Instruction_new(OP_DIV(0,1,2)),
    Instruction_new(OP_RET(0))
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

void test_machine_test__send(void)
{
  Instruction *instructions[] = {
    Instruction_new(OP_LOADI(0,1)), // receiver
    Instruction_new(OP_LOADS(1,2)), // message
    Instruction_new(OP_LOADI(2,1)), // argstart
    Instruction_new(OP_SEND(0,1,2)),
    Instruction_new(OP_RET(0))
  };

  Object *result = test_run_instructions(instructions, 5);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 60);
  cl_assert(result->refcount > 0);

  Object_destroy(result);
}
