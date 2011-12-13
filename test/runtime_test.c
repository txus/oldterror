#include <stdio.h>
#include "clay.h"
#include "../src/runtime.h"
#include "../src/object.h"

void test_runtime_test__call(void)
{
  Object *receiver = Integer_new(50);
  int argc = 1;
  Object *argv[argc];
  argv[0] = Integer_new(100);

  Object *result = call(receiver, "add", argv, argc);

  cl_assert(result);
  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 150);

  Object_destroy(receiver)
  Object_destroy(argv[0]);
  Object_destroy(result);
}

void test_runtime_test__call_kernel_method(void)
{
  int argc = 1;
  Object *argv[argc];
  argv[0] = Integer_new(100);

  init_runtime();
  Object *result = call(MainObject, "print", argv, argc);

  cl_assert(result);
  cl_assert(result->type == tNil);

  Object_destroy(argv[0]);
  Object_destroy(result);
  destroy_runtime();
}
