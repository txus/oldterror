#include "clay.h"
#include "../src/gc.h"
#include "../src/object.h"

void test_gc_test__retain(void)
{
  Object *object = Object_new();
  retain(object);
  cl_assert(object->refcount == 1);
}

void test_gc_test__release(void)
{
  Object *object = Object_new();
  object->refcount = 3;

  release(object);

  cl_assert(object->refcount == 2);
}
