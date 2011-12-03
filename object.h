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

#endif
