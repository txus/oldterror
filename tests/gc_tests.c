#include "minunit.h"
#include <terror/object.h>
#include <terror/gc.h>
#include <assert.h>

static Object *object = NULL;

char *test_retain()
{
  object = Object_new();
  retain(object);
  retain(object);

  mu_assert(object->refcount == 2, "GC retain didn't increment refcount");

  return NULL;
}

char *test_release()
{
  release(object);
  mu_assert(object->refcount == 1, "GC release didn't decrement refcount");

  release(object);
  mu_assert(!object->type, "GC release didn't destroy object");

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_retain);
  mu_run_test(test_release);

  return NULL;
}

RUN_TESTS(all_tests);
