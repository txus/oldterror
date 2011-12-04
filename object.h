#ifndef __object_h__
#define __object_h__

#include "slot.h"
#include "vmmethod.h"

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
  union {
    char *string;
    int integer;
  } value;
  Slot *slots[10];
} Object;

Object *Object_new();
Object *Integer_new(int value);
Object *String_new(char *value);

Object *True_new();
Object *False_new();
Object *Nil_new();

Object *Main_new();

char Object_is_true(Object *self);

void Object_print(Object *object);
VMMethod* Object_lookup_method(Object *receiver, char *method);

#endif
