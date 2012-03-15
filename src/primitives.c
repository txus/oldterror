#include <assert.h>
#include "primitives.h"

void
check_type(Object* object, int type) {
  assert(object->type == type);
}

Object*
Primitive_add(Object *left, Object *right)
{
  check_type(left, tInteger);
  check_type(right, tInteger);

  int a, b;

  a = left->value.integer;
  b = right->value.integer;

  return Integer_new(a + b);
}

Object*
Primitive_sub(Object *left, Object *right)
{
  check_type(left, tInteger);
  check_type(right, tInteger);

  int a, b;
  a = left->value.integer;
  b = right->value.integer;

  return Integer_new(a - b);
}

Object*
Primitive_mul(Object *left, Object *right)
{
  check_type(left, tInteger);
  check_type(right, tInteger);

  int a, b;
  a = left->value.integer;
  b = right->value.integer;

  return Integer_new(a * b);
}

Object*
Primitive_div(Object *left, Object *right)
{
  check_type(left, tInteger);
  check_type(right, tInteger);

  int a, b;
  a = left->value.integer;
  b = right->value.integer;

  assert(b != 0);

  return Integer_new(a / b);
}
