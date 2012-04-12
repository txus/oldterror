#include <string.h>
#include "clay.h"
#include "../src/object.h"

void test_object_test__new(void)
{
  Object *object = Object_new();

  cl_assert(object->type == tObject);

  Object_destroy(object);
}

void test_object_test__True_new(void)
{
  Object *True = True_new();

  cl_assert(True->type == tTrue);

  free(True);
}

void test_object_test__False_new(void)
{
  Object *False = False_new();

  cl_assert(False->type == tFalse);

  free(False);
}

void test_object_test__Nil_new(void)
{
  Object *Nil = Nil_new();

  cl_must_pass(Nil);
  cl_must_pass(Nil->type == tNil);

  free(Nil);
}

void test_object_test__Main_new(void)
{
  Object *Main = Main_new();

  cl_assert(Main->type == tObject);

  free(Main);
}

void test_object_test__print(void)
{
  Object *object  = Object_new();
  Object *integer = Integer_new(3);
  Object *string  = String_new("foo");
  Object *True    = True_new();
  Object *False   = False_new();
  Object *Nil     = Nil_new();

  Object_print(object);
  Object_print(integer);
  Object_print(string);
  Object_print(True);
  Object_print(False);
  Object_print(Nil);

  Object_destroy(object);
  Object_destroy(integer);
  Object_destroy(string);
  free(True);
  free(False);
  free(Nil);
}

void test_object_test__is_true(void)
{
  Object *False  = False_new();
  Object *Nil    = Nil_new();
  Object *object = Object_new();

  cl_assert(Object_is_true(object) == 1);
  cl_assert(Object_is_true(False) == 0);
  cl_assert(Object_is_true(Nil) == 0);

  free(False);
  free(Nil);
  Object_destroy(object);
}

void test_object_lookup_method(void)
{
  Object *object = Object_new();

  Instruction *instructions[] = {
    Instruction_new(0x50000000),
    Instruction_new(0x90000000),
  };

  Object_define_method(object, 0, "foo", instructions, 1);

  VMMethod *method = Object_lookup_method(object, "foo");

  cl_assert(method->instructions[0]->opcode == LOADLOCAL);

  Object_destroy(object);
}

void test_object_test__String_new_assigns_value(void)
{
  Object *string = String_new("foo");

  cl_assert(string->type == tString);
  cl_assert(strcmp(string->value.string, "foo") == 0);

  Object_destroy(string);
}

void test_object_test__Integer_new_assigns_value(void)
{
  Object *integer = Integer_new(3);

  cl_assert(integer->type == tInteger);
  cl_assert(integer->value.integer == 3);

  Object_destroy(integer);
}

void test_object_test__Integer_new_has_add_method(void)
{
  Object *integer = Integer_new(3);

  Slot *slot = integer->slots[0];

  cl_assert(strcmp(slot->name, "add") == 0);
  cl_assert(slot->value.method->instructions[0]->opcode == LOADSELF);

  Object_destroy(integer);
}

void test_object_test__define_method(void)
{
  Object *object = Object_new();

  Instruction *instructions[] = {
    Instruction_new(0x50000000),
    Instruction_new(0x90000000),
  };

  Object_define_method(object, 0, "foo", instructions, 1);

  Slot *slot = object->slots[0];

  cl_assert(strcmp(slot->name, "foo") == 0);
  cl_assert(slot->value.method);
  cl_assert(slot->value.method->instructions[0]->opcode == LOADLOCAL);

  Object_destroy(object);
}
