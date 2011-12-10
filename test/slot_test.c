#include <string.h>
#include "clay.h"
#include "../src/slot.h"

void test_slot_test__new(void)
{
  Slot *slot = Slot_new("foo");

  cl_assert(slot);
  cl_assert(strcmp(slot->name, "foo") == 0);
}
