#ifndef _terror__slot_h_
#define _terror__slot_h_

#include <terror/vmmethod.h>
#include <terror/bstrlib.h>

typedef struct {
  bstring name;
  union {
    VMMethod *method;
  } value;
} Slot;

Slot* Slot_new(bstring name);
void Slot_destroy(Slot *slot);

#endif
