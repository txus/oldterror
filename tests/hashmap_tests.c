#include "minunit.h"
#include <terror/hashmap.h>
#include <terror/object.h>
#include <assert.h>
#include <terror/bstrlib.h>

#define TYPE -1

Hashmap *map = NULL;
static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");

static int traverse_good_cb(HashmapNode *node)
{
  debug("KEY: %s", bdata((bstring)node->key));
  traverse_called++;
  return 0;
}

static int traverse_fail_cb(HashmapNode *node)
{
  debug("KEY: %s", bdata((bstring)node->key));
  traverse_called++;

  if(traverse_called == 2) {
    return 1;
  } else {
    return 0;
  }
}

char *test_create()
{
  map = Hashmap_create(NULL, NULL);
  mu_assert(map != NULL, "Failed to create map.");

  return NULL;
}

char *test_destroy()
{
  Hashmap_destroy(map);

  return NULL;
}

char *test_get_set()
{
  int rc = Hashmap_set(map, &test1, &expect1, TYPE);
  mu_assert(rc == 0, "Failed to set &test1");
  bstring result = Hashmap_get(map, &test1);
  mu_assert(result == &expect1, "Wrong value for test1.");

  rc = Hashmap_set(map, &test2, &expect2, TYPE);
  mu_assert(rc == 0, "Failed to set test2");
  result = Hashmap_get(map, &test2);
  mu_assert(result == &expect2, "Wrong value for test2.");

  rc = Hashmap_set(map, &test3, &expect3, TYPE);
  mu_assert(rc == 0, "Failed to set test3");
  result = Hashmap_get(map, &test3);
  mu_assert(result == &expect3, "Wrong value for test3.");

  return NULL;
}

char *test_get_set_terror_strings()
{
  Hashmap *object_map = Hashmap_create(String_compare, String_hash);
  Object *str_test1   = String_new(bfromcstr("hello"));
  Object *value_test1 = Integer_new(123);
  Object *str_test2   = String_new(bfromcstr("goodbye"));
  Object *value_test2 = Integer_new(321);

  int rc = Hashmap_set(object_map, str_test1, value_test1, TYPE);
  mu_assert(rc == 0, "Failed to set str_test1");
  Object *result = Hashmap_get(object_map, str_test1);
  mu_assert(result->value.integer == 123, "Wrong value for str_test1.");

  rc = Hashmap_set(object_map, str_test2, value_test2, TYPE);
  mu_assert(rc == 0, "Failed to set str_test2");
  result = Hashmap_get(object_map, str_test2);
  mu_assert(result->value.integer == 321, "Wrong value for str_test2.");

  Object_destroy(str_test1);
  Object_destroy(str_test2);
  Object_destroy(value_test1);
  Object_destroy(value_test2);
  Hashmap_destroy(object_map);
  return NULL;
}

char *test_traverse()
{
  int rc = Hashmap_traverse(map, traverse_good_cb);
  mu_assert(rc == 0, "Failed to traverse.");
  mu_assert(traverse_called == 3, "Wrong count traverse.");

  traverse_called = 0;
  rc = Hashmap_traverse(map, traverse_fail_cb);
  mu_assert(rc == 1, "Failed to traverse.");
  mu_assert(traverse_called == 2, "Wrong count traverse for fail.");

  return NULL;
}

char *test_delete()
{
  bstring deleted = (bstring)Hashmap_delete(map, &test1);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect1, "Should get test1");
  bstring result = Hashmap_get(map, &test1);
  mu_assert(result == NULL, "Should delete.");

  deleted = (bstring)Hashmap_delete(map, &test2);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect2, "Should get test2");
  result = Hashmap_get(map, &test2);
  mu_assert(result == NULL, "Should delete.");

  deleted = (bstring)Hashmap_delete(map, &test3);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect3, "Should get test3");
  result = Hashmap_get(map, &test3);
  mu_assert(result == NULL, "Should delete.");

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_get_set);
  mu_run_test(test_get_set_terror_strings);
  mu_run_test(test_traverse);
  mu_run_test(test_delete);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
