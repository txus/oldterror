#ifndef __primitives_h__
#define __primitives_h__

#include <assert.h>
#include "object.h"

void check_type(Object *object, int type);

Object* Primitive_add(Object *, Object *);
Object* Primitive_sub(Object *, Object *);
Object* Primitive_mul(Object *, Object *);
Object* Primitive_div(Object *, Object *);

#endif
