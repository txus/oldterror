#include <stdlib.h>
#include "object.h"
#include "gc.h"
#include "dbg.h"

Object* retain(Object *self) {
  self->refcount++;
  return self;
}

void release(Object *self) {
  self->refcount--;
  if (self->refcount <= 0) {
    Object_destroy(self);
  }
}
