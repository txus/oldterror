#include <stdlib.h>
#include <terror/vmmethod.h>
#include <terror/dbg.h>

VMMethod* VMMethod_new(Instruction **instructions, long *literals, short arity)
{
  VMMethod *method = calloc(1, sizeof(VMMethod));
  check_mem(method);

  method->instructions = instructions;
  method->literals = literals;
  method->arity = arity;

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

