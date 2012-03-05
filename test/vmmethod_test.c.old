#include "clay.h"
#include "../src/vmmethod.h"
#include "../src/object.h"

Object* VMMethod_execute(VMMethod *method, Object **locals, Object *self);

static VMMethod *test_create_method()
{
  long literals[] = {
    (long) "foo",
    (long) 30,
  };

  byte instructions[] = {
    PUSH_STRING, 0,
    PUSH_SELF,
    PUSH_INT, 1,
    RET,
  };

  int count = (sizeof(instructions) / sizeof(byte)) + 1;
  byte *allocated_instructions = allocate_instructions(instructions, count);

  VMMethod *method = VMMethod_new(allocated_instructions, allocate_literals(literals, 2));
  return method;
}

void test_vmmethod_test__new(void)
{
  /* VMMethod *method = test_create_method(); */

  /* cl_must_pass(method); */
  /* cl_must_pass(*(method->start_ip) == PUSH_STRING); */
  /* cl_must_pass(*(method->literals) == (long) "foo"); */

  /* VMMethod_destroy(method); */
}

void test_vmmethod_test__execute(void)
{
  /* Object *self = Main_new(); */
  /* Object *locals[10]; */

  /* VMMethod *method = test_create_method(); */

  /* Object *result = VMMethod_execute(method, locals, self); */

  /* cl_must_pass(result->value.integer == 30); */

  /* VMMethod_destroy(method); */
  /* Object_destroy(result); */
  /* free(self); */
}
