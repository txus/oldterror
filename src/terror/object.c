#include <stdio.h>
#include <stdlib.h>
#include <terror/dbg.h>
#include <terror/object.h>
#include <assert.h>

Object *Object_new()
{
  Object *object = calloc(1, sizeof(Object));
  check_mem(object);

  object->type = tObject;

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

    free(object);
  }
}

Object *Integer_new(int value)
{
  Object *object = calloc(1, sizeof(Object));
  check_mem(object);

  object->type = tInteger;
  object->value.integer = value;

  return object;

error:
  return NULL;
}

Object *String_new(const char *value)
{
  check(value != NULL, "Cannot allocate string with empty value.");

  Object *object = calloc(1, sizeof(Object));
  check_mem(object);

  object->type = tString;
  object->value.string = bfromcstr(value);

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
