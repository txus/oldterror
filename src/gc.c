#include <stdlib.h>
#include "object.h"
#include "gc.h"

Object* retain(Object *self) {
  self->refcount++;
  return self;
}

void release(Object *self) {
  self->refcount--;
  if (self->refcount <= 0)
    free(self);
}
