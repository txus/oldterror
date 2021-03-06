#include "minunit.h"
#include <terror/machine.h>
#include <terror/runtime.h>
#include <terror/gc.h>
#include <assert.h>

#define MACHINE(A) Machine_new(instructions, A, literals, 4, locals, 1, 4)

#define MAX_INSTRUCTIONS 10
#define DESTROY_INSTRUCTIONS() { \
  int i=1; \
  for(i=1; i < MAX_INSTRUCTIONS; i++) { \
    if (instructions[i]) Instruction_destroy(instructions[i]); \
  } \
 }

static Machine *machine = NULL;
static Instruction **instructions = NULL;
static long *literals = NULL;
static Object **locals = NULL;
static Object *self = NULL;

void setup_method()
{
  self = Integer_new(999);

  instructions = calloc(MAX_INSTRUCTIONS, sizeof(Instruction*));

  literals = calloc(4, sizeof(long));
  literals[0] = 123;
  literals[1] = (long)"print";
  literals[2] = (long)"+";
  literals[3] = (long)"price";

  locals = calloc(1, sizeof(Object*));
  locals[0] = Integer_new(444);
}

void teardown_method()
{
  Object_destroy(self);
  free(literals);

  free(instructions);

  Object_destroy(locals[0]);
  free(locals);
}

char *test_new()
{
  instructions[0] = Instruction_new(OP_LOADNIL(0));
  instructions[1] = Instruction_new(OP_RET(0));

  machine = MACHINE(2);

  mu_assert(machine->instructions[0] == instructions[0], "Failed assigning instructions.");
  mu_assert(machine->literals[0] == literals[0], "Failed assigning literals.");
  mu_assert(machine->locals[0] == locals[0], "Failed assigning locals.");

  Machine_destroy(machine);

  DESTROY_INSTRUCTIONS();
  return NULL;
}

char *test_move()
{
  instructions[0] = Instruction_new(OP_LOADI(0, 0));
  instructions[1] = Instruction_new(OP_MOVE(1, 0));
  instructions[2] = Instruction_new(OP_RET(1));

  machine = MACHINE(3);

  Object *result = Machine_run(machine, self);

  mu_assert(result->value.integer == 123, "MOVE failed.");

  Machine_destroy(machine);
  Object_destroy(result);

  DESTROY_INSTRUCTIONS();
  return NULL;
}

char *test_loadi()
{
  instructions[0] = Instruction_new(OP_LOADI(0, 0));
  instructions[1] = Instruction_new(OP_RET(0));

  machine = MACHINE(2);

  Object *result = Machine_run(machine, self);

  mu_assert(result->value.integer == 123, "MOVE or LOADI failed.");

  Machine_destroy(machine);
  Object_destroy(result);

  DESTROY_INSTRUCTIONS();
  return NULL;
}

char *test_loads()
{
  instructions[0] = Instruction_new(OP_LOADS(0, 1));
  instructions[1] = Instruction_new(OP_RET(0));

  bstring literal = bfromcstr("print");

  machine = MACHINE(2);

  Object *result = Machine_run(machine, self);

  mu_assert(bstrcmp(result->value.string, literal) == 0, "LOADS failed.");

  Machine_destroy(machine);
  Object_destroy(result);
  bdestroy(literal);

  DESTROY_INSTRUCTIONS();
  return NULL;
}

char *test_loadnil()
{
  instructions[0] = Instruction_new(OP_LOADNIL(0));
  instructions[1] = Instruction_new(OP_RET(0));

  machine = MACHINE(2);

  Object *result = Machine_run(machine, self);

  mu_assert(result->type == tNil, "LOADNIL failed.");

  Machine_destroy(machine);

  DESTROY_INSTRUCTIONS();
  return NULL;
}

char *test_loadbool()
{
  instructions[0] = Instruction_new(OP_LOADBOOL(0, 1));
  instructions[1] = Instruction_new(OP_RET(0));

  machine = MACHINE(2);

  Object *result = Machine_run(machine, self);

  mu_assert(result->type == tTrue, "LOADBOOL failed.");

  Machine_destroy(machine);

  DESTROY_INSTRUCTIONS();
  return NULL;
}

char *test_jmp()
{
  instructions[0] = Instruction_new(OP_LOADI(0, 0));
  instructions[1] = Instruction_new(OP_JMP(2));
  instructions[2] = Instruction_new(OP_LOADS(0, 1));
  instructions[3] = Instruction_new(OP_RET(0));

  machine = MACHINE(4);

  Object *result = Machine_run(machine, self);

  mu_assert(result->type == tInteger, "JMP failed.");
  mu_assert(result->value.integer == 123, "JMP failed.");

  Machine_destroy(machine);

  Object_destroy(result);
  DESTROY_INSTRUCTIONS();
  return NULL;
}

char *test_jif()
{
  instructions[0] = Instruction_new(OP_LOADBOOL(0, 0));
  instructions[1] = Instruction_new(OP_JIF(2, 0));
  instructions[2] = Instruction_new(OP_LOADS(0, 1));
  instructions[3] = Instruction_new(OP_RET(0));

  machine = MACHINE(4);

  Object *result = Machine_run(machine, self);

  mu_assert(result->type == tFalse, "JIF failed.");

  Machine_destroy(machine);

  DESTROY_INSTRUCTIONS();
  return NULL;
}

char *test_jit()
{
  instructions[0] = Instruction_new(OP_LOADBOOL(0, 1));
  instructions[1] = Instruction_new(OP_JIT(2, 0));
  instructions[2] = Instruction_new(OP_LOADS(0, 1));
  instructions[3] = Instruction_new(OP_RET(0));

  machine = MACHINE(4);

  Object *result = Machine_run(machine, self);

  mu_assert(result->type == tTrue, "JIT failed.");

  Machine_destroy(machine);

  DESTROY_INSTRUCTIONS();
  return NULL;
}

char *test_loadself()
{
  instructions[0] = Instruction_new(OP_LOADSELF(0));
  instructions[1] = Instruction_new(OP_RET(0));

  machine = MACHINE(2);
  Object *result = Machine_run(machine, self);

  mu_assert(result == self, "LOADSELF failed.");

  Machine_destroy(machine);

  Instruction_destroy(instructions[0]);
  Instruction_destroy(instructions[1]);
  return NULL;
}

char *test_loadlocal()
{
  instructions[0] = Instruction_new(OP_LOADLOCAL(0, 0));
  instructions[1] = Instruction_new(OP_RET(0));

  machine = MACHINE(2);
  Object *result = Machine_run(machine, self);

  mu_assert(result->value.integer == 444, "LOADLOCAL failed.");

  Machine_destroy(machine);

  Instruction_destroy(instructions[0]);
  Instruction_destroy(instructions[1]);
  return NULL;
}

char *test_setlocal()
{
  instructions[0] = Instruction_new(OP_LOADI(0, 0));
  instructions[1] = Instruction_new(OP_SETLOCAL(0, 1));
  instructions[2] = Instruction_new(OP_RET(0));

  machine = MACHINE(3);
  Object *result = Machine_run(machine, self);

  mu_assert(locals[1]->value.integer == 123, "SETLOCAL failed.");

  Machine_destroy(machine);

  Object_destroy(result);
  Instruction_destroy(instructions[0]);
  Instruction_destroy(instructions[1]);
  Instruction_destroy(instructions[2]);
  return NULL;
}

char *test_loadslot()
{
  instructions[0] = Instruction_new(OP_LOADSELF(0));
  instructions[1] = Instruction_new(OP_LOADS(1, 3));
  instructions[2] = Instruction_new(OP_LOADSLOT(2, 0, 1));
  instructions[3] = Instruction_new(OP_RET(2));

  bstring slotname = bfromcstr("price");

  Object *value = Integer_new(literals[0]);

  Object_register_slot(self, slotname, value);

  machine = MACHINE(4);
  Object *result = Machine_run(machine, self);

  mu_assert(value == result, "LOADSLOT failed.");

  Machine_destroy(machine);

  Object_delete_slot(self, slotname);

  bdestroy(slotname);

  Object_destroy(value);

  Instruction_destroy(instructions[0]);
  Instruction_destroy(instructions[1]);
  Instruction_destroy(instructions[2]);
  Instruction_destroy(instructions[3]);
  return NULL;
}

char *test_setslot()
{
  instructions[0] = Instruction_new(OP_LOADSELF(0));
  instructions[1] = Instruction_new(OP_LOADS(1, 3));
  instructions[2] = Instruction_new(OP_LOADI(2, 0));
  instructions[3] = Instruction_new(OP_SETSLOT(0, 1, 2));
  instructions[4] = Instruction_new(OP_RET(0));

  bstring slotname = bfromcstr("price");

  machine = MACHINE(5);
  Object *result = Machine_run(machine, self);
  Object *slot_value = Object_get_slot(result, slotname);

  mu_assert(slot_value->value.integer == 123, "SETSLOT failed.");

  Machine_destroy(machine);

  bdestroy(slotname);

  Instruction_destroy(instructions[0]);
  Instruction_destroy(instructions[1]);
  Instruction_destroy(instructions[2]);
  Instruction_destroy(instructions[3]);
  Instruction_destroy(instructions[4]);
  return NULL;
}

char *test_makearray()
{
  instructions[0] = Instruction_new(OP_LOADSELF(1));
  instructions[1] = Instruction_new(OP_LOADS(2, 1));
  instructions[2] = Instruction_new(OP_LOADI(3, 0));
  instructions[3] = Instruction_new(OP_MAKEARRAY(0, 1, 3));
  instructions[4] = Instruction_new(OP_RET(0));

  machine = MACHINE(5);
  Object *result = Machine_run(machine, self);
  DArray *array = (DArray*)result->value.other;

  mu_assert(DArray_count(array) == 3, "Array has not a length of 3.");

  Object *e_self = (Object*)DArray_at(array, 0);
  Object *e_str  = (Object*)DArray_at(array, 1);
  Object *e_int  = (Object*)DArray_at(array, 2);

  printf("Refcount for self: %i\n", e_self->refcount);
  printf("Refcount for str: %i\n", e_str->refcount);
  printf("Refcount for int: %i\n", e_int->refcount);

  bstring print = bfromcstr("print");

  mu_assert(result->type == tArray, "Array type is not array.");
  mu_assert(e_self == self, "First array element is not self.");
  mu_assert(bstrcmp(e_str->value.string, print) == 0, "Second array element is not 'print'.");
  mu_assert(e_int->value.integer == 123, "Third array element is not 123.");

  Machine_destroy(machine);
  Object_destroy(result);

  bdestroy(print);

  Instruction_destroy(instructions[0]);
  Instruction_destroy(instructions[1]);
  Instruction_destroy(instructions[2]);
  Instruction_destroy(instructions[3]);
  Instruction_destroy(instructions[4]);
  return NULL;
}


char *test_send()
{
  instructions[0] = Instruction_new(OP_LOADI(0, 0));
  instructions[1] = Instruction_new(OP_LOADS(1, 2));
  instructions[2] = Instruction_new(OP_LOADI(2, 0));
  instructions[3] = Instruction_new(OP_SEND(0, 1, 2));
  instructions[4] = Instruction_new(OP_RET(0));

  machine = MACHINE(5);
  Object *result = Machine_run(machine, self);

  mu_assert(result->value.integer == 246, "SEND failed.");

  Machine_destroy(machine);
  Object_destroy(result);

  Instruction_destroy(instructions[0]);
  Instruction_destroy(instructions[1]);
  Instruction_destroy(instructions[2]);
  Instruction_destroy(instructions[3]);
  Instruction_destroy(instructions[4]);
  return NULL;
}

char *test_dump()
{
  instructions[0] = Instruction_new(OP_LOADNIL(0));
  instructions[1] = Instruction_new(OP_DUMP());
  instructions[2] = Instruction_new(OP_RET(0));

  machine = MACHINE(3);
  Machine_run(machine, self);

  Machine_destroy(machine);

  Instruction_destroy(instructions[0]);
  Instruction_destroy(instructions[1]);
  Instruction_destroy(instructions[2]);
  return NULL;
}

char *all_tests() {
  mu_suite_start();

  setup_method();
  mu_run_test(test_new);

  mu_run_test(test_move);
  mu_run_test(test_loadi);
  mu_run_test(test_loads);

  Runtime_init(); // for nil and booleans
  mu_run_test(test_loadnil);
  mu_run_test(test_loadbool);

  mu_run_test(test_jmp);
  mu_run_test(test_jif);
  mu_run_test(test_jit);

  mu_run_test(test_loadself);

  mu_run_test(test_loadlocal);
  mu_run_test(test_setlocal);

  mu_run_test(test_loadslot);
  mu_run_test(test_setslot);

  mu_run_test(test_makearray);

  mu_run_test(test_send);
  mu_run_test(test_dump);

  Runtime_destroy(); // for nil and booleans
  teardown_method();

  return NULL;
}

RUN_TESTS(all_tests);
