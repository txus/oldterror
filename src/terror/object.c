#include <stdio.h>
#include <stdlib.h>
#include <terror/dbg.h>
#include <terror/object.h>
#include <terror/runtime.h>
#include <assert.h>

Object *MainObject;

Object *Object_new()
{
  Object *object = calloc(1, sizeof(Object));
  check_mem(object);

  object->type = tObject;
  object->refcount = 0;
  object->immortal = 0;

  object->slots = calloc(10, sizeof(Slot*));

  // Initialize slots to NULL
  int i = 0;
  for(i = 0; i < 10; i++) {
    object->slots[i] = NULL;
  }

  return object;

error:
  return NULL;
}

void Object_destroy(Object *object)
{
  if(object != NULL) {
    if(object->immortal == 1) return;

    if(object->type == tString) {
      bdestroy(object->value.string);
    }

    int i = 0;
    for(i=0; i < 10; i++) {
      if(object->slots[i]) Slot_destroy(object->slots[i]);
    }
    if(object->slots) free(object->slots);

    free(object);
  }
}

void Object_define_method(Object *object, int idx, bstring name, Instruction **instructions, int instructions_count, short arity) {
  long *literals = calloc(10, sizeof(long));
  VMMethod *method = VMMethod_new(instructions, instructions_count, literals, 10, arity);

  Slot *slot = Slot_new(name);
  slot->value.method = method;

  debug("Defined method %s with arity %i\n", bdata(name), method->arity);

  object->slots[idx] = slot;
}

Object *Integer_new(int value)
{
  Object *object = Object_new();

  object->type = tInteger;
  object->value.integer = value;

  /* // Define #add */
  /* Instruction **instructions = calloc(4, sizeof(Instruction*)); */
  /* instructions[0] = Instruction_new(OP_LOADSELF(1)); */
  /* instructions[1] = Instruction_new(OP_LOADLOCAL(2, 0)); */
  /* instructions[2] = Instruction_new(OP_ADD(0, 1, 2)); */
  /* instructions[3] = Instruction_new(OP_RET(0)); */
  /* Object_define_method(object, 0, bfromcstr("add"), instructions, 4, 1); */

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
  int i = 0;

  for (i = 0; i < 10; i++) {
    if (object->slots[i]) {
      if (bstrcmp(object->slots[i]->name, name) == 0) {
        return object->slots[i]->value.method->arity;
      }
    }
  }

  if (object == MainObject) {
    return 1;
  }

  return -2;
}

VMMethod* Object_lookup_method(Object *object, bstring name) {
  int i = 0;
  VMMethod *method = NULL;

  if (object == MainObject) {
    method = calloc(1, sizeof(VMMethod));
    method->arity = 1;
    return method;
  }

  for (i = 0; i < 10; i++) {
    if (object->slots[i]) {
      if (bstrcmp(object->slots[i]->name, name) == 0) {
        method = object->slots[i]->value.method;
        break;
      }
    }
  }

  debug(" Returning legit method with arity %i, and its id is %p\n ", method->arity, method);
  return method;
}
