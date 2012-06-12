#ifndef _terror__gc_h_
#define _terror__gc_h_

#include <terror/object.h>

Object* retain(Object *self);
void release(Object *self);

#endif
