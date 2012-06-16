#include "minunit.h"
#include <terror/native_method.h>
#include <assert.h>

static NativeMethod *method = NULL;

static Object* native_add(void *a, void *b, void *c) {
  Object *self    = (Object*)a;
  Object *operand = (Object*)b;

  return Integer_new(self->value.integer + operand->value.integer);
}

char *test_create() {
  method = NativeMethod_new(native_add, 1);
  return NULL;
}

char *test_execute() {
  Object *number1 = Integer_new(20);
  Object *number2 = Integer_new(10);

  Object **locals = calloc(1, sizeof(Object*));
  locals[0] = number2;

  Object *result = NativeMethod_execute(method, locals, 1, 0, number1);

  mu_assert(result->value.integer == 30, "Native method failed to execute.");

  Object_destroy(result);
  Object_destroy(number1);
  Object_destroy(number2);
  free(locals);
  return NULL;
}

void cleanup() {
  NativeMethod_destroy(method);
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_execute);
  cleanup();

  return NULL;
}

RUN_TESTS(all_tests);
