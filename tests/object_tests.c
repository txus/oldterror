#include "minunit.h"
#include <terror/object.h>
#include <assert.h>

static Object *object = NULL;
static Object *parent = NULL;
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

char *test_create_with_parent() {
  parent = Object_new();
  object = Object_new_with_parent(parent);

  mu_assert(object->parent == parent, "Object didn't get assigned a parent.");

  return NULL;
}

char *test_destroy()
{
  Object_destroy(object);
  Object_destroy(parent);

  Object_destroy(immortal);
  mu_assert(immortal->immortal == 1, "Immortal object was destroyed.");

  // Cleanup immortal object manually
  Object_destroy_immortal(immortal);
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

char *test_arrays()
{
  Object **contents = calloc(4, sizeof(Object*));
  contents[0] = Integer_new(0);
  contents[1] = Integer_new(1);
  contents[2] = Integer_new(2);
  contents[3] = Integer_new(3);

  Object *object = Array_new(contents, 4);

  mu_assert(object->type == tArray, "Array has the wrong type");
  DArray *array = (DArray*)object->value.other;
  Object *first = (Object*)DArray_at(array, 0);
  mu_assert(first->value.integer == 0, "Array element is wrong.");

  Object_destroy(object);
  free(contents);
  return NULL;
}

char *test_hashes()
{
  Object **contents = calloc(4, sizeof(Object*));
  contents[0] = String_new(bfromcstr("foo"));
  contents[1] = Integer_new(1);
  contents[2] = String_new(bfromcstr("bar"));
  contents[3] = Integer_new(2);

  Object *array = Array_new(contents, 4);
  Object *object = Hash_new(array);

  mu_assert(object->type == tHash, "Hash has the wrong type");
  Hashmap *map = (Hashmap*)object->value.other;
  Object *foo = (Object*)Hashmap_get(map, contents[0]);
  mu_assert(foo->value.integer == 1, "Hash element 'foo' is wrong.");
  Object *bar = (Object*)Hashmap_get(map, contents[2]);
  mu_assert(bar->value.integer == 2, "Hash element 'bar' is wrong.");

  Object_destroy(object);
  Object_destroy(array);
  free(contents);
  return NULL;
}

char *test_slots()
{
  Object *parent_age   = Integer_new(50);
  Object *parent_money = Integer_new(1000);
  Object *child_age    = Integer_new(25);

  Object *parent = Object_new();
  Object *child  = Object_new_with_parent(parent);

  bstring money = bfromcstr("money");
  bstring age   = bfromcstr("age");

  Object_register_slot(parent, bfromcstr("money"), parent_money);
  Object_register_slot(parent, bfromcstr("age"), parent_age);
  Object_register_slot(child, bfromcstr("age"), child_age);

  mu_assert(Object_lookup_slot(parent, age)->value.integer == 50, "Parent age is wrong");
  mu_assert(Object_lookup_slot(child, age)->value.integer == 25, "Child age is wrong");
  mu_assert(Object_lookup_slot(parent, money)->value.integer == 1000, "Parent money is wrong");
  mu_assert(Object_lookup_slot(child, money)->value.integer == 1000, "Child money is wrong");

  Object_destroy(child);

  bdestroy(money);
  bdestroy(age);
  return NULL;
}

char *test_special()
{
  object = True_new();
  mu_assert(object->type == tTrue, "True has the wrong type");
  mu_assert(object->immortal == 1, "True is not immortal");
  Object_destroy_immortal(object);

  object = False_new();
  mu_assert(object->type == tFalse, "False has the wrong type");
  mu_assert(object->immortal == 1, "False is not immortal");
  Object_destroy_immortal(object);

  object = Nil_new();
  mu_assert(object->type == tNil, "Nil has the wrong type");
  mu_assert(object->immortal == 1, "Nil is not immortal");
  Object_destroy_immortal(object);

  object = Lobby_new();
  mu_assert(object->type == tObject, "Lobby has the wrong type");
  mu_assert(object->immortal == 1, "Lobby is not immortal");
  Object_destroy_immortal(object);

  return NULL;
}

char *test_print()
{
  object = Object_new();
  Object_print(object);
  Object_destroy(object);

  object = True_new();
  Object_print(object);
  Object_destroy_immortal(object);

  object = False_new();
  Object_print(object);
  Object_destroy_immortal(object);

  object = Nil_new();
  Object_print(object);
  Object_destroy_immortal(object);

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
  mu_run_test(test_arrays);
  mu_run_test(test_hashes);

  mu_run_test(test_slots);

  mu_run_test(test_special);
  mu_run_test(test_print);

  return NULL;
}

RUN_TESTS(all_tests);
