#include <stdlib.h>
#include <terror/gc.h>

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
