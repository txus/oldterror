#include <stdlib.h>
#include "object.h"

/*
Public: Allocates a new Object.

Returns a pointer to the newly allocated object.
*/
Object *Object_new() {
  return malloc(sizeof(Object));
}
