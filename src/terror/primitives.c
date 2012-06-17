#include <terror/primitives.h>
#include <assert.h>

Object *Primitive_add(void *a, void *b, void *c)
{
  Object *left  = (Object*)a;
  Object *right = (Object*)b;

  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  return Integer_new(left->value.integer + right->value.integer);
}

Object *Primitive_sub(void *a, void *b, void *c)
{
  Object *left  = (Object*)a;
  Object *right = (Object*)b;

  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  return Integer_new(left->value.integer - right->value.integer);
}

Object *Primitive_mul(void *a, void *b, void *c)
{
  Object *left  = (Object*)a;
  Object *right = (Object*)b;

  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  return Integer_new(left->value.integer * right->value.integer);
}

Object *Primitive_div(void *a, void *b, void *c)
{
  Object *left  = (Object*)a;
  Object *right = (Object*)b;

  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  assert(right->value.integer != 0 && "Cannot divide by zero");

  return Integer_new(left->value.integer / right->value.integer);
}
