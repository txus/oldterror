#ifndef __object_h__
#define __object_h__

#include "slot.h"
#define MAX_SLOTS 10

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
  Slot *slots[MAX_SLOTS];
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

char Object_is_true(Object *self);

void Object_print(Object *object);
void Object_define_method(Object *object, int idx, const char *name, byte instructions[]);

VMMethod* Object_lookup_method(Object *receiver, const char *method);

#endif
