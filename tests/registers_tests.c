#include "minunit.h"
#include <terror/object.h>
#include <terror/registers.h>
#include <assert.h>

char *test_print()
{
  Object **registers = calloc(3, sizeof(Object*));
  registers[0] = Object_new();
  registers[1] = Integer_new(123);
  registers[2] = String_new("hello");

  Registers_print(registers, 3);

  Object_destroy(registers[0]);
  Object_destroy(registers[1]);
  Object_destroy(registers[2]);

  free(registers);
  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_print);

  return NULL;
}

RUN_TESTS(all_tests);
