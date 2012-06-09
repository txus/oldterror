#include "minunit.h"
#include <terror/bstrlib.h>
#include <terror/slot.h>
#include <assert.h>

static Slot *slot = NULL;

char *test_new()
{
  bstring name = bfromcstr("foo");
  slot = Slot_new(name);

  mu_assert(bstrcmp(slot->name, name) == 0, "Failed assigning name to slot.");

  return NULL;
}

char *test_destroy()
{
  Slot_destroy(slot);
  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_new);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
