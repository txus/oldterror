#ifndef __slot_h__
#define __slot_h__

#include "vmmethod.h"
#include "object.h"

typedef struct {
  char *name;
  union {
    VMMethod *method;
    // Object *object;
  } value;
} Slot;

#endif
