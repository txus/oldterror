#include "clay.h"
#include "../src/stack.h"
#include "../src/gc.h"

void test_stack_test__print(void)
{
  Object *foo = Integer_new(30);
  Object *bar = String_new("foo");
  Object *nil = Nil_new();

  Object *stack[10];
  Object **sp = stack;

  STACK_PUSH(foo);
  STACK_PUSH(bar);
  STACK_PUSH(nil);

  Stack_print(stack, sp);

  Object_destroy(foo);
  Object_destroy(bar);
  free(nil);
}

void test_stack_test__cleanup(void)
{
  Object *foo = Integer_new(30);
  foo->refcount = 1;
  Object *bar = String_new("foo");
  bar->refcount = 2;

  Object *stack[10];
  Object **sp = stack;

  STACK_PUSH(foo);
  STACK_PUSH(bar);

  Stack_cleanup(stack, sp);

  cl_assert(foo->refcount == 1);
  cl_assert(bar->refcount == 3);

  Object_destroy(foo);
  Object_destroy(bar);
}
