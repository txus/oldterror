#ifndef __stack_h__
#define __stack_h__
#include <assert.h>
#include "object.h"

#define STACK_MAX 10
#define STACK_PUSH(I) { assert(sp - stack < STACK_MAX); *(++sp) = (I); }
#define STACK_POP()   ( *sp-- )
#define STACK_PEEK()   ( *sp )

void Stack_print(Object **, Object **);
#endif
