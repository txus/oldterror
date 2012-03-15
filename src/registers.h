#ifndef __registers_h__
#define __registers_h__
#include "object.h"

#define NUM_REGISTERS 10
#define REGISTER(R, V) { Object *obj = (V); retain(obj); (R) = obj; }
#define CLEAN_REGISTER(R) { if (R) release(R); }

void Registers_print(Object **);
void Registers_cleanup(Object **);
#endif
