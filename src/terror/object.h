#ifndef _terror__object_h_
#define _terror__object_h_

#include <terror/bstrlib.h>
#include <terror/instruction.h>
#include <terror/hashmap.h>
#include <terror/darray.h>

struct vmmethod_s;

typedef enum {
  tObject,
  tInteger,
  tString,
  tArray,
  tFunction,
  tTrue,
  tFalse,
  tNil
} Type;

typedef struct {
  Type type;
  unsigned char immortal;
  union {
    int integer;
    bstring string;
    void *other;
  } value;
  Hashmap *slots;
  int refcount;
} Object;

Object *Object_new();
void Object_destroy(Object *object);
void Object_destroy_immortal(Object *object);
void Object_print(Object *object);
int Object_lookup_method_arity(Object *object, bstring name);
struct vmmethod_s* Object_lookup_method(Object *object, bstring name);
void Object_define_method(Object *object, bstring name, Instruction **instructions, int instructions_count, short arity);
int Object_register_slot(Object *receiver, bstring slot_name, Object *value);
void* Object_delete_slot(Object *receiver, bstring slot_name);
Object* Object_get_slot(Object *receiver, bstring slot_name);

Object *Integer_new(int value);
Object *String_new(bstring value);
Object *Function_new(Instruction **instructions, int instructions_count, short arity);
Object *Array_new(Object** contents, int count);
Object *True_new();
Object *False_new();
Object *Nil_new();
Object *Main_new();

#endif
