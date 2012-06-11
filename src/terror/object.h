#ifndef _terror__object_h_
#define _terror__object_h_

#include <terror/bstrlib.h>
#include <terror/instruction.h>

struct slot_s;
struct vmmethod_s;

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
  struct slot_s **slots;
  int refcount;
} Object;

Object *Object_new();
void Object_destroy(Object *object);
void Object_print(Object *object);
int Object_lookup_method_arity(Object *object, bstring name);
struct vmmethod_s* Object_lookup_method(Object *object, bstring name);
void Object_define_method(Object *object, int idx, bstring name, Instruction **instructions, int instructions_count, short arity);

Object *Integer_new(int value);
Object *String_new(bstring value);
Object *True_new();
Object *False_new();
Object *Nil_new();
Object *Main_new();

#endif
