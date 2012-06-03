#include <terror/primitives.h>
#include <assert.h>

Object *Primitive_add(Object *left, Object *right)
{
  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  int a, b;

  a = left->value.integer;
  b = right->value.integer;

  return Integer_new(a + b);
}

Object *Primitive_sub(Object *left, Object *right)
{
  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  int a, b;
  a = left->value.integer;
  b = right->value.integer;

  return Integer_new(a - b);
}

Object *Primitive_mul(Object *left, Object *right)
{
  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  int a, b;
  a = left->value.integer;
  b = right->value.integer;

  return Integer_new(a * b);
}

Object *Primitive_div(Object *left, Object *right)
{
  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  int a, b;
  a = left->value.integer;
  b = right->value.integer;

  assert(b != 0 && "Cannot divide by zero");

  return Integer_new(a / b);
}
