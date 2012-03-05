#ifndef __registers_h__
#define __registers_h__
#include <assert.h>
#include "object.h"

#define NUM_REGISTERS 10
#define REGISTER(R, V) { Object *obj = (V); retain(obj); (R) = obj; }

void Registers_print(Object **);
void Registers_cleanup(Object **);
#endif
