#include <stdlib.h>
#include "slot.h"

Slot* Slot_new(char *name) {
  Slot *slot = malloc(sizeof(Slot));
  slot->name = name;
  return slot;
}
