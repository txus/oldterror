#include <stdlib.h>
#include <string.h>
#include <terror/slot.h>
#include <terror/dbg.h>

Slot* Slot_new(bstring name) {
  Slot *slot = calloc(1, sizeof(Slot));
  check_mem(slot);

  slot->name = name;
  slot->value.method = NULL;
  return slot;

error:
  return NULL;
}

void Slot_destroy(Slot *slot) {
  if (slot->name) bdestroy(slot->name);
  if (slot->value.method) {
    VMMethod_destroy(slot->value.method);
  }
  free(slot);
}
