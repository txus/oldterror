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

struct object_s {
  Type type;
  unsigned char immortal;
  unsigned char native;
  union {
    int integer;
    bstring string;
    void *other;
  } value;
  Hashmap *slots;
  struct object_s *parent;
  int refcount;
};

typedef struct object_s Object;

typedef Object* (*native_fn)(void*, void*, void*);

Object *Object_new();
Object *Object_new_with_parent(Object *parent);
void Object_destroy(Object *object);
void Object_destroy_immortal(Object *object);
void Object_print(Object *object);
int Object_lookup_method_arity(Object *object, bstring name);
struct vmmethod_s* Object_lookup_method(Object *object, bstring name);
void Object_define_native_method(Object *object, bstring name, native_fn native_function, short arity);
void Object_define_method(Object *object, bstring name, Instruction **instructions, int instructions_count, short arity);
int Object_register_slot(Object *receiver, bstring slot_name, Object *value);
void* Object_delete_slot(Object *receiver, bstring slot_name);
Object* Object_get_slot(Object *receiver, bstring slot_name);

Object *Integer_new(int value);
Object *String_new(bstring value);
Object *Function_native_new(native_fn function, short arity);
Object *Function_new(Instruction **instructions, int instructions_count, short arity);
Object *Array_new(Object **contents, int count);
Object *True_new();
Object *False_new();
Object *Nil_new();
Object *Lobby_new();

#endif
