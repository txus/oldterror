#include <terror/primitives.h>
#include <assert.h>

void
check_type(Object* object, int type) {
  assert(object->type == type && "Type mismatch in primitive operation");
}

Object *Primitive_add(Object *left, Object *right)
{
  check_type(left, tInteger);
  check_type(right, tInteger);

  int a, b;

  a = left->value.integer;
  b = right->value.integer;

  return Integer_new(a + b);
}

Object *Primitive_sub(Object *left, Object *right)
{
  check_type(left, tInteger);
  check_type(right, tInteger);

  int a, b;
  a = left->value.integer;
  b = right->value.integer;

  return Integer_new(a - b);
}

Object *Primitive_mul(Object *left, Object *right)
{
  check_type(left, tInteger);
  check_type(right, tInteger);

  int a, b;
  a = left->value.integer;
  b = right->value.integer;

  return Integer_new(a * b);
}

Object *Primitive_div(Object *left, Object *right)
{
  check_type(left, tInteger);
  check_type(right, tInteger);

  int a, b;
  a = left->value.integer;
  b = right->value.integer;

  assert(b != 0 && "Cannot divide by zero");

  return Integer_new(a / b);
}
