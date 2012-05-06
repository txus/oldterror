#ifndef _terror__object_h_
#define _terror__object_h_

#include <terror/bstrlib.h>

enum {
  tObject,
  tInteger,
  tString,
  tTrue,
  tFalse,
  tNil
};

typedef struct {
  unsigned char type;
  unsigned char immortal;
  union {
    int integer;
    bstring string;
  } value;
  int refcount;
} Object;

Object *Object_new();
void Object_destroy(Object *object);
void Object_print(Object *object);

Object *Integer_new(int value);
Object *String_new(const char *value);
Object *True_new();
Object *False_new();
Object *Nil_new();
Object *Main_new();

#endif
