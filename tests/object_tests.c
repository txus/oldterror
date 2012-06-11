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

  immortal = True_new();
  mu_assert(object != NULL, "Failed to create immortal object.");

  return NULL;
}

char *test_destroy()
{
  Object_destroy(object);

  Object_destroy(immortal);
  mu_assert(immortal->immortal == 1, "Immortal object was destroyed.");

  // Cleanup immortal object manually
  free(immortal);
  return NULL;
}

char *test_integers()
{
  object = Integer_new(123);
  mu_assert(object->type == tInteger, "Integer has the wrong type");
  mu_assert(object->value.integer == 123, "Integer value wasn't set correctly");

  Object_destroy(object);
  return NULL;
}

char *test_strings()
{
  bstring bstr = bfromcstr("123");
  object = String_new(bstr);
  mu_assert(object->type == tString, "String has the wrong type");
  mu_assert(bstrcmp(object->value.string, bstr) == 0, "String value wasn't set correctly");

  Object_destroy(object);
  return NULL;
}

char *test_special()
{
  object = True_new();
  mu_assert(object->type == tTrue, "True has the wrong type");
  mu_assert(object->immortal == 1, "True is not immortal");
  free(object);

  object = False_new();
  mu_assert(object->type == tFalse, "False has the wrong type");
  mu_assert(object->immortal == 1, "False is not immortal");
  free(object);

  object = Nil_new();
  mu_assert(object->type == tNil, "Nil has the wrong type");
  mu_assert(object->immortal == 1, "Nil is not immortal");
  free(object);

  object = Main_new();
  mu_assert(object->type == tObject, "Main has the wrong type");
  mu_assert(object->immortal == 1, "Main is not immortal");
  free(object);

  return NULL;
}

char *test_print()
{
  object = Object_new();
  Object_print(object);
  Object_destroy(object);

  object = True_new();
  Object_print(object);
  free(object);

  object = False_new();
  Object_print(object);
  free(object);

  object = Nil_new();
  Object_print(object);
  free(object);

  object = Integer_new(123);
  Object_print(object);
  Object_destroy(object);

  bstring hello = bfromcstr("hello");
  object = String_new(hello);
  Object_print(object);
  Object_destroy(object);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_destroy);

  mu_run_test(test_integers);
  mu_run_test(test_strings);

  mu_run_test(test_special);
  mu_run_test(test_print);

  return NULL;
}

RUN_TESTS(all_tests);
