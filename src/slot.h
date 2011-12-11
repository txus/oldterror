#ifndef __slot_h__
#define __slot_h__

#include "vmmethod.h"

typedef struct {
  char *name;
  union {
    VMMethod *method;
  } value;
} Slot;

Slot* Slot_new(const char *name);
void Slot_destroy(Slot *slot);

#endif
