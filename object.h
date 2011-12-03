#ifndef __object_h__
#define __object_h__

enum { tObject, tInteger, tString };

typedef struct {
  char type;
  union {
    char *string;
    int integer;
  } value;
} Object;

Object *Object_new();
Object *Integer_new(int value);
Object *String_new(char *value);
void Object_print(Object *);

#endif
