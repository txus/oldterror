#include <terror/primitives.h>
#include <terror/runtime.h>
#include <assert.h>

/*
 * Generic primitives
 */

Object*
Primitive_print(void *_, void *b, void *__)
{
  Object_print((Object*)b);
  return NilObject;
}

Object*
Primitive_puts(void *a, void *b, void *c)
{
  Primitive_print(a, b, c);
  printf("\n");
  return NilObject;
}

/*
 * Object primitives
 */

Object*
Primitive_Object_clone(void *a, void *_, void *__)
{
  Object *self = (Object*)a;
  return Object_new_with_parent(self);
}

// TODO: Move into VM.primitive(:hash, [a,b,c,d])
Object*
Primitive_Object_hash(void *_, void *b, void *__)
{
  Object *array = (Object*)b;
  assert(array->type == tArray && "Cannot create Hash from non-array.");
  return Hash_new(array);
}

/*
 * Integer primitives
 */

Object *Primitive_Integer_add(void *a, void *b, void *_)
{
  Object *left  = (Object*)a;
  Object *right = (Object*)b;

  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  return Integer_new(left->value.integer + right->value.integer);
}

Object *Primitive_Integer_sub(void *a, void *b, void *_)
{
  Object *left  = (Object*)a;
  Object *right = (Object*)b;

  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  return Integer_new(left->value.integer - right->value.integer);
}

Object *Primitive_Integer_mul(void *a, void *b, void *_)
{
  Object *left  = (Object*)a;
  Object *right = (Object*)b;

  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  return Integer_new(left->value.integer * right->value.integer);
}

Object *Primitive_Integer_div(void *a, void *b, void *_)
{
  Object *left  = (Object*)a;
  Object *right = (Object*)b;

  CHECK_TYPE(left, tInteger);
  CHECK_TYPE(right, tInteger);

  assert(right->value.integer != 0 && "Cannot divide by zero");

  return Integer_new(left->value.integer / right->value.integer);
}

/*
 * Array primitives
 */

Object* Primitive_Array_at(void *a, void *b, void *_) {
  Object *self  = (Object*)a;
  int idx       = ((Object*)b)->value.integer;
  DArray *array = (DArray*)self->value.other;

  if(idx >= DArray_count(array)) {
    return NilObject;
  } else {
    return (Object*)DArray_at(array, idx);
  }
}

/*
 * Hash primitives
 */

Object* Primitive_Hash_get(void *a, void *b, void *_) {
  Object *self  = (Object*)a;
  Object *key   = (Object*)b;
  Hashmap *hash = (Hashmap*)self->value.other;

  return Hashmap_get(hash, key);
}

