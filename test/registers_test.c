#include <stdio.h>
#include "clay.h"
#include "../src/registers.h"
#include "../src/gc.h"

void test_registers_test__print(void)
{
  Object *foo = Integer_new(30);
  Object *bar = String_new("foo");
  Object *nil = Nil_new();

  Object *registers[10];

  registers[0] = foo;
  registers[1] = bar;
  registers[2] = nil;

  Registers_print(registers);

  Object_destroy(foo);
  Object_destroy(bar);
  free(nil);
}

void test_registers_test__cleanup(void)
{
  Object **registers = { Nil_new() };

  REGISTER(registers[0], Integer_new(30));
  REGISTER(registers[1], String_new("foo"));

  Object *foo = registers[0];
  Object *bar = registers[1];

  foo->refcount++; // 2
  bar->refcount++;
  bar->refcount++; // 3

  Registers_cleanup(registers);

  cl_assert(foo->refcount == 2);
  cl_assert(bar->refcount == 2);

  Object_destroy(foo);
  Object_destroy(bar);
}
