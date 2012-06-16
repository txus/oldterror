#include <stdio.h>
#include <stdlib.h>
#include <terror/dbg.h>
#include <terror/object.h>
#include <terror/vmmethod.h>
#include <terror/runtime.h>
#include <terror/gc.h>
#include <assert.h>

Object *MainObject;

Object *Object_new()
{
  Object *object = calloc(1, sizeof(Object));
  check_mem(object);

  object->type = tObject;
  object->refcount = 0;
  object->immortal = 0;

  object->slots = Hashmap_create(NULL, NULL);

  return object;

error:
  return NULL;
}

static int delete_node(HashmapNode *node) {
  bdestroy((bstring)node->key);
  release((Object*)node->data);
  return 0;
}

void Object_destroy(Object *object)
{
  if(object != NULL) {
    if(object->immortal == 1) return;

    if(object->type == tString) {
      bdestroy(object->value.string);
    }
    if(object->type == tFunction) {
      VMMethod_destroy((VMMethod*)object->value.other);
    }
    if(object->type == tArray) {
      DArray *array = (DArray*)object->value.other;
      int i=0;
      for(i=0; i < DArray_count(array); i++) {
        release((Object*)DArray_at(array, i));
      }
      DArray_destroy(array);
    }

    if(object->slots) {
      Hashmap_traverse(object->slots, delete_node);
      Hashmap_destroy(object->slots);
    }

    free(object);
  }
}

void Object_destroy_immortal(Object *object) {
  if(object != NULL) {
    if(object->type == tString) {
      bdestroy(object->value.string);
    }

    if(object->slots) Hashmap_destroy(object->slots);

    free(object);
  }
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

  // Define #add
  Instruction **instructions = calloc(4, sizeof(Instruction*));
  instructions[0] = Instruction_new(OP_LOADSELF(1));
  instructions[1] = Instruction_new(OP_LOADLOCAL(2, 0));
  instructions[2] = Instruction_new(OP_ADD(0, 1, 2));
  instructions[3] = Instruction_new(OP_RET(0));
  Object_define_method(object, bfromcstr("add"), instructions, 4, 1);

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

Object*
Array_new(Object **contents, int count) {
  DArray *array = DArray_create(sizeof(Object*), count);
  int i=0;
  for(i=0; i < count; i++) {
    retain((Object*)contents[i]);
    DArray_push(array, contents[i]);
  }

  Object *object      = Object_new();
  object->type        = tArray;
  object->value.other = array;

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

Object *Main_new()
{
  Object *object = calloc(1, sizeof(Object));
  check_mem(object);

  object->type = tObject;
  object->immortal = 1;

  object->slots = Hashmap_create(NULL, NULL);

  return object;

error:
  return NULL;
}

void Object_print(Object* object) {
  assert(object != NULL && "Cannot print null object");

  switch(object->type) {
    case tInteger:
      printf("#<tInteger:%p @value=%i>", object, object->value.integer);
      break;
    case tString:
      printf("#<tString:%p @value=\"%s\">", object, bdata(object->value.string));
      break;
    case tArray:
      printf("#<tArray:%p @contents=[", object);
      DArray *array = (DArray*)object->value.other;

      int i = 0, count = DArray_count(array);
      for(i=0; i < count; i++) {
        Object_print((Object*)DArray_at(array, i));
        if (i+1 != count) printf(", ");
      }

      printf("]>");
      break;
    case tFunction:
      printf("#<tFunction:%p @method=\"%p\">", object, object->value.other);
      break;
    case tTrue:
      printf("#<tTrue:%p>", object);
      break;
    case tFalse:
      printf("#<tFalse:%p>", object);
      break;
    case tNil:
      printf("#<tNil:%p>", object);
      break;
    case tObject:
      printf("#<tObject:%p>", object);
      break;
  }
}

int Object_lookup_method_arity(Object *object, bstring name) {
  if (object == MainObject) {
    return 1;
  }

  Object *fn = (Object*)Hashmap_get(object->slots, name);
  VMMethod *method = (VMMethod*)fn->value.other;
  return method->arity;
}

VMMethod* Object_lookup_method(Object *object, bstring name) {
  VMMethod *method = NULL;

  if (object == MainObject) {
    method = calloc(1, sizeof(VMMethod));
    method->arity = 1;
    return method;
  } else {
    Object *fn = (Object*)Hashmap_get(object->slots, name);
    method = (VMMethod*)fn->value.other;
  }

  return method;
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
