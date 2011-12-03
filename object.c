#include <stdlib.h>
#include <stdio.h>
#include "object.h"

/*
Public: Allocates a new Object.

Returns a pointer to the newly allocated object.
*/
Object *Object_new() {
  return malloc(sizeof(Object));
}

/*
Public: Allocates a new Integer object.

int value - the value of the Integer.

Returns a pointer to the object.
*/
Object *Integer_new(int value) {
  Object *integer = Object_new();
  integer->type = tInteger;
  integer->value.integer = value;
  return integer;
}

void Object_print(Object* object) {
  switch(object->type) {
    case tInteger:
      printf("#<tInteger:%p @value=%i>", object, object->value.integer);
      break;
  }
}
