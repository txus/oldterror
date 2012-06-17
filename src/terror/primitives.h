#ifndef _terror__primitives_h_
#define _terror__primitives_h_

#include <terror/object.h>
#include <assert.h>

void check_type(Object *object, int type);

#define CHECK_TYPE(O, T) assert((O)->type == (T) && "Type mismatch in primitive operation")

// Generic primitive methods
Object* Primitive_print(void*, void*, void*);
Object* Primitive_puts(void*, void*, void*);

// Object primitive methods
Object* Primitive_Object_clone(void*, void*, void*);
Object* Primitive_Object_hash(void*, void*, void*);

// Integer primitive methods
Object* Primitive_Integer_add(void*, void*, void*);
Object* Primitive_Integer_sub(void*, void*, void*);
Object* Primitive_Integer_mul(void*, void*, void*);
Object* Primitive_Integer_div(void*, void*, void*);

// Array primitive methods
Object* Primitive_Array_at(void *a, void *b, void *_);

// Hash primitive methods
Object* Primitive_Hash_get(void *a, void *b, void *_);

#endif

