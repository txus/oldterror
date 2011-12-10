#include "clay.h"
#include "../src/machine.h"

static VMMethod* test_create_method_for_machine()
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

  byte *allocated_instructions = allocate_instructions(instructions);

  VMMethod *method = VMMethod_new(allocated_instructions, literals);
  return method;
}

static Machine* test_create_machine()
{
  VMMethod *method = test_create_method_for_machine();
  Object **locals;
  Machine *machine = Machine_new(method->start_ip, method->literals, locals);
  return machine;
}

void test_machine_test__new(void)
{
  Machine *machine = test_create_machine();
  cl_assert(*(machine->ip) == PUSH_STRING);
}
