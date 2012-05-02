#ifndef _terror__object_h_
#define _terror__object_h_

enum {
  tObject,
  tInteger,
  tString,
  tTrue,
  tFalse,
  tNil
};

typedef struct {
  char type;
  char immortal;
  union {
    char *string;
    int integer;
  } value;
  int refcount;
} Object;

Object *Object_new();
void Object_destroy(Object *object);

Object *Integer_new(int value);
Object *String_new(const char *value);
Object *True_new();
Object *False_new();
Object *Nil_new();
Object *Main_new();

#endif
