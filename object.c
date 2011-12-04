#include <stdlib.h>
#include <stdio.h>
#include "object.h"

/*
Public: Allocates a new Object.

Returns a pointer to the newly allocated object.
*/
Object *Object_new() {
  Object *object = malloc(sizeof(Object));
  object->type = tObject;
  return object;
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
Public: Allocates a new True object.

Returns a pointer to the object.
*/
Object *True_new() {
  Object *true_object = Object_new();
  true_object->type = tTrue;
  true_object->value.integer = 1;
  return true_object;
}

/*
Public: Allocates a new False object.

Returns a pointer to the object.
*/
Object *False_new() {
  Object *false_object = Object_new();
  false_object->type = tFalse;
  false_object->value.integer = 0;
  return false_object;
}

/*
Public: Allocates a new Nil object.

Returns a pointer to the object.
*/
Object *Nil_new() {
  Object *nil_object = Object_new();
  nil_object->type = tNil;
  nil_object->value.integer = 0;
  return nil_object;
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
