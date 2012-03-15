#include "primitives.h"

Object*
Primitive_add(Object *left, Object *right)
{
  int num_left, num_right;
  num_left  = left->value.integer;
  num_right = right->value.integer;

  return Integer_new(num_left + num_right);
}

Object*
Primitive_sub(Object *left, Object *right)
{
  int num_left, num_right;
  num_left  = left->value.integer;
  num_right = right->value.integer;

  return Integer_new(num_left - num_right);
}

Object*
Primitive_mul(Object *left, Object *right)
{
  int num_left, num_right;
  num_left  = left->value.integer;
  num_right = right->value.integer;

  return Integer_new(num_left * num_right);
}

Object*
Primitive_div(Object *left, Object *right)
{
  int num_left, num_right;
  num_left  = left->value.integer;
  num_right = right->value.integer;

  return Integer_new(num_left / num_right);
}
