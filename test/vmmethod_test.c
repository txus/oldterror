#include "clay.h"
#include "../src/vmmethod.h"
#include "../src/object.h"

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

  VMMethod *method = VMMethod_new(instructions, literals);
  return method;
}

void test_vmmethod_test__new(void)
{
  VMMethod *method = test_create_method();

  cl_must_pass(method);
  cl_must_pass(*(method->start_ip) == PUSH_STRING);
  cl_must_pass(*(method->literals) == (long) "foo");
}

void test_vmmethod_test__execute(void)
{
  Object *self = Main_new();
  Object **locals;

  VMMethod *method = test_create_method();

  Object *result = VMMethod_execute(method, locals, self);

  cl_must_pass(result->value.integer == 30);
}
