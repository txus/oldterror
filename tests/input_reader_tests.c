#include "minunit.h"
#include <terror/bstrlib.h>
#include <terror/input_reader.h>
#include <assert.h>

static BytecodeFile *file = NULL;
static bstring filename   = NULL;
static bstring method     = NULL;

void setup_bytecode_file() {
  filename = bfromcstr("tests/hello_world.tvm");
  method   = bfromcstr("main");
  file     = BytecodeFile_new(filename);
}

char *test_parses_header()
{
  mu_assert(bstrcmp(file->filename, filename) == 0, "Failed loading file.");
  mu_assert(bstrcmp(file->method, method) == 0, "Failed parsing method name.");

  mu_assert(file->registers_count == 10, "Failed parsing register count.");
  mu_assert(file->locals_count == 2, "Failed parsing locals count.");
  mu_assert(file->literals_count == 4, "Failed parsing literals count.");
  mu_assert(file->instructions_count == 17, "Failed parsing instructions count.");

  return NULL;
}

char *test_parses_literals()
{
  mu_assert(file->literals[0] == 123, "Failed parsing literal 123.");
  mu_assert(strcmp((char*)file->literals[1], "print") == 0, "Failed parsing literal 'print'.");
  mu_assert(strcmp((char*)file->literals[2], "Goodbye world!") == 0, "Failed parsing literal 'Goodbye world!'.");
  mu_assert(strcmp((char*)file->literals[3], "Hello world!") == 0, "Failed parsing literal 'Hello world!'.");

  return NULL;
}

char *test_parses_instructions()
{
  Instruction *i = file->instructions[5];

  mu_assert(i->opcode == 0x21, "Failed to parse instruction opcode.")
  mu_assert(i->fields.a == 0x06, "Failed to parse instruction operand A.");
  mu_assert(i->fields.b == 0x02, "Failed to parse instruction operand B.");
  mu_assert(i->fields.c == 0x00, "Failed to parse instruction operand C.");

  return NULL;
}

char *test_destroy()
{
  BytecodeFile_destroy(file);
  return NULL;
}

char *all_tests() {
  mu_suite_start();

  setup_bytecode_file();
  mu_run_test(test_parses_header);
  mu_run_test(test_parses_literals);
  mu_run_test(test_parses_instructions);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
