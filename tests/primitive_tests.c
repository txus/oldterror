#include "minunit.h"
#include <terror/primitives.h>
#include <assert.h>

static Object *foo = NULL;
static Object *bar = NULL;
static Object *result = NULL;

void setup()
{
  foo = Integer_new(8);
  bar = Integer_new(2);
}

void teardown()
{
  Object_destroy(foo);
  Object_destroy(bar);
  Object_destroy(result);
}

char *test_add()
{
  setup();

  result = Primitive_add(foo, bar, NULL);
  mu_assert(result->value.integer == 10, "Primitive add failed.");

  teardown();

  return NULL;
}

char *test_sub()
{
  setup();

  result = Primitive_sub(foo, bar, NULL);
  mu_assert(result->value.integer == 6, "Primitive sub failed.");

  teardown();

  return NULL;
}

char *test_mul()
{
  setup();

  result = Primitive_mul(foo, bar, NULL);
  mu_assert(result->value.integer == 16, "Primitive mul failed.");

  teardown();

  return NULL;
}

char *test_div()
{
  setup();

  result = Primitive_div(foo, bar, NULL);
  mu_assert(result->value.integer == 4, "Primitive div failed.");

  teardown();

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_add);
  mu_run_test(test_sub);
  mu_run_test(test_mul);
  mu_run_test(test_div);

  return NULL;
}

RUN_TESTS(all_tests);
