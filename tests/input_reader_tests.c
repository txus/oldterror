#include "minunit.h"
#include <terror/input_reader.h>
#include <assert.h>

char *test_read_file()
{
  BytecodeFile *file = BytecodeFile_new("tests/hello_world.tvm");

  mu_assert(strcmp(file->filename, "tests/hello_world.tvm") == 0, "Failed loading file.");

  BytecodeFile_destroy(file);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_read_file);

  return NULL;
}

RUN_TESTS(all_tests);
