#include "minunit.h"
#include <terror/instruction.h>
#include <terror/vmmethod.h>
#include <assert.h>

static VMMethod *method = NULL;
static long *literals = NULL;
static Instruction **instructions = NULL;

void setup()
{
  literals = calloc(3, sizeof(long));
  literals[0] = (long)"123";
  literals[1] = (long)123;
  literals[2] = (long)"foo";

  instructions = calloc(3, sizeof(Instruction*));
  instructions[0] = Instruction_new(0x11223344);
  instructions[1] = Instruction_new(0x22334455);
  instructions[2] = Instruction_new(0x33445566);
}

char *test_create()
{
  method = VMMethod_new(instructions, 3, literals, 3, 3);
  mu_assert(method != NULL, "VMMethod wasn't created correctly.");
  mu_assert(method->literals == literals, "Literals weren't assigned correctly")
  mu_assert(method->literals_count == 3, "Literals count wasn't assigned correctly")

  mu_assert(method->instructions == instructions, "Instructions weren't assigned correctly")
  mu_assert(method->instructions_count == 3, "Instructions count wasn't assigned correctly")
  mu_assert(method->arity == 3, "Arity wasn't assigned correctly")

  return NULL;
}

char *test_destroy()
{
  VMMethod_destroy(method);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  setup();
  mu_run_test(test_create);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
