#include <stdlib.h>
#include <terror/vmmethod.h>
#include <terror/machine.h>
#include <terror/dbg.h>

VMMethod* VMMethod_new(Instruction **instructions, int instructions_count, long *literals, int literals_count, short arity)
{
  VMMethod *method = calloc(1, sizeof(VMMethod));
  check_mem(method);

  method->instructions       = instructions;
  method->instructions_count = instructions_count;
  method->literals           = literals;
  method->literals_count     = literals_count;
  method->arity              = arity;

  return method;

error:
  return NULL;
}

void VMMethod_destroy(VMMethod *method)
{
  if (method != NULL)
  {
    if (method->literals != NULL) free(method->literals);

    free(method);
  }
}

Object* VMMethod_execute(VMMethod *method, Object **locals, int locals_count, int registers_count, Object *self) {
  Machine *machine = Machine_new(method->instructions, method->instructions_count, method->literals, method->literals_count, locals, locals_count, registers_count);
  Object *result = Machine_run(machine, self);
  Machine_destroy(machine);
  return result;
}
