#include <stdlib.h>
#include <terror/object.h>

Object *Object_new()
{
  Object *object = calloc(1, sizeof(Object));
  return object;
}
