#ifndef _terror__primitives_h_
#define _terror__primitives_h_

#include <terror/object.h>
#include <assert.h>

void check_type(Object *object, int type);

#define CHECK_TYPE(O, T) assert((O)->type == (T) && "Type mismatch in primitive operation")

Object* Primitive_add(void*, void*, void*);
Object* Primitive_sub(void*, void*, void*);
Object* Primitive_mul(void*, void*, void*);
Object* Primitive_div(void*, void*, void*);

#endif

