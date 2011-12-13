#ifndef __gc_h__
#define __gc_h__

#include "object.h"

Object* retain(Object *self);
void release(Object *self);

#endif
