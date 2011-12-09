#include "clay.h"
#include "../src/stack.h"

void test_stack_test__print(void)
{
  Object *foo = Integer_new(30);
  Object *bar = String_new("foo");
  Object *nil = Nil_new();

  Object *stack[10];
  stack[0] = foo;
  stack[1] = bar;
  stack[2] = nil; // top of the stack

  Object **sp = &stack[2]; // Set the pointer to the top of the stack

  Stack_print(stack, sp);
}
