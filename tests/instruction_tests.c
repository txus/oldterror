#include "minunit.h"
#include <terror/instruction.h>
#include <assert.h>

static Instruction *instruction = NULL;

char *test_create()
{
  instruction = Instruction_new(0x11223344);

  mu_assert(instruction->opcode == 0x11, "Incorrect instruction opcode");
  mu_assert(instruction->fields.a == 0x22, "Incorrect instruction field a");
  mu_assert(instruction->fields.b == 0x33, "Incorrect instruction field b");
  mu_assert(instruction->fields.c == 0x44, "Incorrect instruction field c");

  return NULL;
}

char *test_destroy()
{
  Instruction_destroy(instruction);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
