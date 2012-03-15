#include <stdlib.h>
#include <string.h>
#include "slot.h"
#include "util.h"

Slot* Slot_new(const char *name) {
  Slot *slot = malloc(sizeof(Slot));
  if (!slot) die("Could not allocate Slot");
  slot->name = strdup(name);
  slot->value.method = NULL;
  return slot;
}

void Slot_destroy(Slot *slot) {
  if (slot->name) free(slot->name);
  if (slot->value.method) {
    VMMethod_destroy(slot->value.method);
  }
  free(slot);
}
