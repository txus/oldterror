#ifndef _terror__primitives_h_
#define _terror__primitives_h_

#include <terror/object.h>

void check_type(Object *object, int type);

Object* Primitive_add(Object *, Object *);
Object* Primitive_sub(Object *, Object *);
Object* Primitive_mul(Object *, Object *);
Object* Primitive_div(Object *, Object *);

#endif

