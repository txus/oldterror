#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "slot.h"
#include "object.h"
#include "opcode.h"
#include "vmmethod.h"
#include "runtime.h"

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

  // DEFINE #add method for integers
  byte add_instructions[] = {
    PUSH_SELF,
    GET_LOCAL, 0,
    ADD,
    RET,
  };

  byte *ip = add_instructions;
  long literals[] = {};
  VMMethod *add = VMMethod_new(ip, literals);

  Slot *slot = Slot_new("add");
  slot->value.method = add;

  integer->slots[0] = slot;

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
Public: Allocates the special Main object

Returns a pointer to the object.
*/
Object *Main_new() {
  Object *main_object = Object_new();
  main_object->type = tObject;

  return main_object;
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
      printf("#<tString:%p>", object);
      /* printf("#<tString:%p @value=\\"%s\\">", object, object->value.string); */
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

char Object_is_true(Object *self) {
  // false and nil are == false,
  // everything else is true.
  if (self == FalseObject || self == NilObject) return 0;
  return 1;
}

VMMethod* Object_lookup_method(Object *object, char *name) {
  int i = 0;
  VMMethod *method;

  for (i = 0; i < 10; i++) {
    if (object->slots[i]) {
      if (strcmp(object->slots[i]->name, name) == 0) {
        method = object->slots[i]->value.method;
        return method;
      }
    }
  }
  return NULL;
}
