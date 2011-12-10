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
}
