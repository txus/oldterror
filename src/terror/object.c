#include <stdio.h>
#include <stdlib.h>
#include <terror/dbg.h>
#include <terror/util.h>
#include <terror/object.h>
#include <terror/vmmethod.h>
#include <terror/native_method.h>
#include <terror/primitives.h>
#include <terror/runtime.h>
#include <terror/gc.h>
#include <assert.h>

Object *Lobby;
Object *NilObject;

Object *Object_new()
{
  Object *object = calloc(1, sizeof(Object));
  check_mem(object);

  object->type = tObject;
  object->refcount = 0;
  object->immortal = 0;
  object->native = 0;
  object->parent = NULL;

  object->slots = Hashmap_create(NULL, NULL);

  return object;

error:
  return NULL;
}

Object *Object_new_with_parent(Object *parent)
{
  Object *object = Object_new();
  object->parent = retain(parent);
  return object;
}

static int delete_node(HashmapNode *node) {
  bdestroy((bstring)node->key);
  release((Object*)node->data);
  return 0;
}

static int delete_object_node(HashmapNode *node) {
  release((Object*)node->key);
  release((Object*)node->data);
  return 0;
}

void Object_destroy(Object *object)
{
  if(object != NULL) {
    if(object->immortal == 1) return;

    switch(object->type) {
      case tString:
        bdestroy(object->value.string);
        break;
      case tFunction:
        if (object->native) {
          NativeMethod_destroy((NativeMethod*)object->value.other);
        } else {
          VMMethod_destroy((VMMethod*)object->value.other);
        }
        break;
      case tArray: {
        DArray *array = (DArray*)object->value.other;
        int i=0;
        for(i=0; i < DArray_count(array); i++) {
          release((Object*)DArray_at(array, i));
        }
        DArray_destroy(array);
        break;
      }
      case tHash: {
        Hashmap *map = (Hashmap*)object->value.other;
        Hashmap_traverse(map, delete_object_node);
        Hashmap_destroy(map);
        break;
      }
      default:
        break;
    }

    if(object->slots) {
      Hashmap_traverse(object->slots, delete_node);
      Hashmap_destroy(object->slots);
    }

    if(object->parent) {
      release(object->parent);
    }

    free(object);
  }
}

void Object_destroy_immortal(Object *object) {
  if(object != NULL) {
    if(object->type == tString) {
      bdestroy(object->value.string);
    }

    if(object->slots) {
      Hashmap_traverse(object->slots, delete_node);
      Hashmap_destroy(object->slots);
    }

    free(object);
  }
}

void Object_define_native_method(Object *object, bstring name, native_fn native_function, short arity) {
  Object *fn = Function_native_new(native_function, arity);
  Hashmap_set(object->slots, name, fn, tFunction);
}

void Object_define_method(Object *object, bstring name, Instruction **instructions, int instructions_count, short arity) {
  Object *fn = Function_new(instructions, instructions_count, arity);
  Hashmap_set(object->slots, name, fn, tFunction);
}

Object *Integer_new(int value)
{
  Object *object = Object_new();

  object->type = tInteger;
  object->value.integer = value;

  Object_define_native_method(object, bfromcstr("+"), Primitive_Integer_add, 1);
  Object_define_native_method(object, bfromcstr("-"), Primitive_Integer_sub, 1);
  Object_define_native_method(object, bfromcstr("*"), Primitive_Integer_mul, 1);
  Object_define_native_method(object, bfromcstr("/"), Primitive_Integer_div, 1);

  return object;
}

Object *String_new(bstring value)
{
  check(value != NULL, "Cannot allocate string with empty value.");

  Object *object = Object_new();

  object->type = tString;
  object->value.string = value;

  return object;

error:
  return NULL;
}

Object*
Function_new(Instruction **instructions, int instructions_count, short arity)
{
  long *literals      = calloc(10, sizeof(long));
  VMMethod *method    = VMMethod_new(instructions, instructions_count, literals, 10, arity);
  Object *object      = Object_new();
  object->type        = tFunction;
  object->value.other = method;

  return object;
}

Object *
Function_native_new(native_fn function, short arity) {
  NativeMethod *method = NativeMethod_new(function, arity);
  Object *object      = Object_new();

  object->native      = 1;
  object->type        = tFunction;
  object->value.other = method;

  return object;
}

Object*
Array_new(Object **contents, int count) {
  DArray *array = DArray_create(sizeof(Object*), count || 1);
  int i=0;
  for(i=0; i < count; i++) {
    retain((Object*)contents[i]);
    DArray_push(array, contents[i]);
  }

  Object *object      = Object_new();
  object->type        = tArray;
  object->value.other = array;

  Object_define_native_method(object, bfromcstr("[]"), Primitive_Array_at, 1);

  return object;
}

Object*
Hash_new(Object *array_obj) {
  DArray *array = (DArray*)array_obj->value.other;
  int count = DArray_count(array);
  assert(count % 2 == 0 && "Hash element count must be even.");

  Hashmap *hash = Hashmap_create(String_compare, String_hash);

  int i=0;
  for(i=0; i < count; i += 2) {
    Object *key   = retain((Object*)DArray_at(array, i));
    Object *value = retain((Object*)DArray_at(array, i+1));
    assert(key->type == tString && "All hash keys must be strings");
    Hashmap_set(hash, key, value, value->type);
  }

  Object *object      = Object_new();
  object->type        = tHash;
  object->value.other = hash;

  Object_define_native_method(object, bfromcstr("[]"), Primitive_Hash_get, 1);

  return object;
}

Object *True_new()
{
  Object *object = calloc(1, sizeof(Object));
  check_mem(object);

  object->type = tTrue;
  object->immortal = 1;

  return object;

error:
  return NULL;
}

Object *False_new()
{
  Object *object = calloc(1, sizeof(Object));
  check_mem(object);

  object->type = tFalse;
  object->immortal = 1;

  return object;

error:
  return NULL;
}

Object *Nil_new()
{
  Object *object = calloc(1, sizeof(Object));
  check_mem(object);

  object->type = tNil;
  object->immortal = 1;

  return object;

error:
  return NULL;
}

Object *Lobby_new()
{
  Object *lobby = calloc(1, sizeof(Object));
  check_mem(lobby);

  lobby->type = tObject;
  lobby->immortal = 1;

  lobby->slots = Hashmap_create(NULL, NULL);
  return lobby;

error:
  return NULL;
}

void Object_print(Object* object) {
  assert(object != NULL && "Cannot print null object");

  switch(object->type) {
    case tInteger:
      printf("%i", object->value.integer);
      break;
    case tString:
      printf("%s", bdata(object->value.string));
      break;
    case tArray:
      printf("[");
      DArray *array = (DArray*)object->value.other;

      int i = 0, count = DArray_count(array);
      for(i=0; i < count; i++) {
        Object_print((Object*)DArray_at(array, i));
        if (i+1 != count) printf(", ");
      }

      printf("]");
      break;
    case tHash:
      printf("#<tHash:%p>", object);
      break;
    case tFunction:
      printf("#<tFunction:%p @method=\"%p\">", object, object->value.other);
      break;
    case tTrue:
      printf("true");
      break;
    case tFalse:
      printf("false");
      break;
    case tNil:
      printf("nil");
      break;
    case tObject:
      printf("#<tObject:%p>", object);
      break;
  }
}

int Object_lookup_method_arity(Object *object, bstring name) {
  Object *fn = Object_lookup_slot(object, name);
  if(!fn) {
    printf("Undefined slot #%s for ", bdata(name));
    Object_print(object);
    die("Could not find slot.");
  }

  if(fn->type != tFunction) { // it is a normal attribute
    return -2;
  }

  if(fn->native) {
    NativeMethod *method = (NativeMethod*)fn->value.other;
    return method->arity;
  } else {
    VMMethod *method = (VMMethod*)fn->value.other;
    return method->arity;
  }
}

int Object_register_slot(Object *receiver, bstring slot_name, Object *value) {
  int rc = Hashmap_set(receiver->slots, slot_name, value, value->type);
  assert(rc == 0 && "Could not register slot.");
  return rc;
}

void* Object_delete_slot(Object *receiver, bstring slot_name) {
  return Hashmap_delete(receiver->slots, slot_name);
}

Object* Object_get_slot(Object *receiver, bstring slot_name) {
  return Hashmap_get(receiver->slots, slot_name);
}

Object* Object_lookup_slot(Object *receiver, bstring slot_name) {
  Object *obj    = receiver;
  Object *result = NULL;

  do {
    result = (Object*)Hashmap_get(obj->slots, slot_name);
    if(result) return result;
  } while((obj = obj->parent));

  return NULL;
}
