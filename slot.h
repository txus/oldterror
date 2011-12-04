#ifndef __slot_h__
#define __slot_h__

#include "vmmethod.h"

typedef struct {
  char *name;
  VMMethod *method;
} Slot;

#endif
