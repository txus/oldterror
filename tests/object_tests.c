#include "minunit.h"
#include <terror/object.h>
#include <assert.h>

static Object *object = NULL;
static Object *immortal = NULL;

char *test_create()
{
  object = Object_new();
  mu_assert(object != NULL, "Failed to create object.");
  mu_assert(object->type == tObject, "Object has an invalid type.");

  immortal = Object_new();
  immortal->immortal = 1;
  mu_assert(object != NULL, "Failed to create immortal object.");

  return NULL;
}

char *test_destroy()
{
  Object_destroy(object);

  Object_destroy(immortal);
  mu_assert(immortal->immortal == 1, "Immortal object was destroyed.");

  return NULL;
}

char *test_integers()
{
  object = Integer_new(123);
  mu_assert(object->type == tInteger, "Integer has the wrong type");
  mu_assert(object->value.integer == 123, "Integer value wasn't set correctly");

  return NULL;
}

char *test_strings()
{
  object = String_new("123");
  mu_assert(object->type == tString, "String has the wrong type");
  mu_assert(biseqcstr(object->value.string, "123") == 1, "String value wasn't set correctly");

  return NULL;
}

char *test_special()
{
  object = True_new();
  mu_assert(object->type == tTrue, "True has the wrong type");
  mu_assert(object->immortal == 1, "True is not immortal");

  object = False_new();
  mu_assert(object->type == tFalse, "False has the wrong type");
  mu_assert(object->immortal == 1, "False is not immortal");

  object = Nil_new();
  mu_assert(object->type == tNil, "Nil has the wrong type");
  mu_assert(object->immortal == 1, "Nil is not immortal");

  object = Main_new();
  mu_assert(object->type == tObject, "Main has the wrong type");
  mu_assert(object->immortal == 1, "Main is not immortal");

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_destroy);

  mu_run_test(test_integers);
  mu_run_test(test_strings);

  mu_run_test(test_special);

  return NULL;
}

RUN_TESTS(all_tests);
