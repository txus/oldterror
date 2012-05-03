#include <stdlib.h>
#include <terror/dbg.h>
#include <terror/object.h>

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

    if(object->value.string != NULL) bdestroy(object->value.string);
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
