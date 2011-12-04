#include <stdlib.h>
#include "vmmethod.h"
#include "object.h"

VMMethod* VMMethod_new(byte **start_ip) {
  VMMethod *method = malloc(sizeof(VMMethod));
  method->start_ip = start_ip;
  return method;
}

/* Object* VMMethod_execute(VMMethod* method, long literals[], Object *locals[]) { */
/*   run(literals, method->start_ip, self); */
/* } */
