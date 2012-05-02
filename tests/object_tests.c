#include "minunit.h"
#include <terror/object.h>
#include <assert.h>

static Object *object = NULL;

char *test_create()
{
  object = Object_new();
  mu_assert(object != NULL, "Failed to create object.");

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);

  return NULL;
}

RUN_TESTS(all_tests);
