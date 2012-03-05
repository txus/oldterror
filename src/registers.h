#ifndef __registers_h__
#define __registers_h__
#include <assert.h>
#include "object.h"

#define NUM_REGISTERS 10

void Registers_print(Object **);
void Registers_cleanup(Object **);
#endif
