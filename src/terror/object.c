#include <stdlib.h>
#include <terror/object.h>

Object *Object_new()
{
  Object *object = calloc(1, sizeof(Object));
  return object;
}

void Object_destroy(Object *object)
{
  if(object != NULL) {
    if(object->value.string != NULL) free(object->value.string);
    free(object);
  }
}
