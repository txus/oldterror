#ifndef __slot_h__
#define __slot_h__

#include "vmmethod.h"

typedef struct {
  char *name;
  union {
    VMMethod *method;
    // Object *object;
  } value;
} Slot;

Slot* Slot_new(char *name);

#endif
