#include "clay.h"
#include "../src/primitives.h"

void test_primitives_test__add(void)
{
  Object *left = Integer_new(10);
  Object *right = Integer_new(25);

  Object *result = Primitive_add(left, right);

  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 35);

  Object_destroy(left);
  Object_destroy(right);
  Object_destroy(result);
}

void test_primitives_test__sub(void)
{
  Object *left = Integer_new(10);
  Object *right = Integer_new(25);

  Object *result = Primitive_sub(left, right);

  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == -15);

  Object_destroy(left);
  Object_destroy(right);
  Object_destroy(result);
}

void test_primitives_test__mul(void)
{
  Object *left = Integer_new(10);
  Object *right = Integer_new(25);

  Object *result = Primitive_mul(left, right);

  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 250);

  Object_destroy(left);
  Object_destroy(right);
  Object_destroy(result);
}

void test_primitives_test__div(void)
{
  Object *left = Integer_new(10);
  Object *right = Integer_new(2);

  Object *result = Primitive_div(left, right);

  cl_assert(result->type == tInteger);
  cl_assert(result->value.integer == 5);

  Object_destroy(left);
  Object_destroy(right);
  Object_destroy(result);
}
