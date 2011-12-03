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

/*
Public: Allocates a new String object.

char *value - the value of the String.

Returns a pointer to the object.
*/
Object *String_new(char *value) {
  Object *string = Object_new();
  string->type = tString;
  string->value.string = value;
  return string;
}

/*
Public: Prints an Object to STDOUT depending on its type.

Object *object - a pointer to the object.

Returns nothing.
*/
void Object_print(Object* object) {
  switch(object->type) {
    case tInteger:
      printf("#<tInteger:%p @value=%i>", object, object->value.integer);
      break;
    case tString:
      printf("#<tString:%p @value=\"%s\">", object, object->value.string);
      break;
  }
}
