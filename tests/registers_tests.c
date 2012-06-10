#include "minunit.h"
#include <terror/object.h>
#include <terror/registers.h>
#include <assert.h>

char *test_print()
{
  bstring hello = bfromcstr("hello");
  Object **registers = calloc(3, sizeof(Object*));
  registers[0] = Object_new();
  registers[1] = Integer_new(123);
  registers[2] = String_new(hello);

  Registers_print(registers, 3);

  Object_destroy(registers[0]);
  Object_destroy(registers[1]);
  Object_destroy(registers[2]);

  free(registers);
  return NULL;
}

char *test_cleanup()
{
  bstring hello = bfromcstr("hello");
  Object **registers = calloc(3, sizeof(Object*));
  registers[0] = Object_new();
  registers[1] = Integer_new(123);
  registers[2] = String_new(hello);

  Registers_cleanup(registers, 3, 1);

  mu_assert(registers[1]->value.integer == 123, "Registers_cleanup wiped wrong register.");

  Object_destroy(registers[1]);
  free(registers);
  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_print);
  mu_run_test(test_cleanup);

  return NULL;
}

RUN_TESTS(all_tests);
