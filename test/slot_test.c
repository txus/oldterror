#include "clay.h"
#include "../src/slot.h"

void test_slot_test__new(void)
{
  Slot *slot = Slot_new("foo");

  cl_must_pass(slot);
  cl_must_pass(strcmp(slot->name, "foo"));
}
