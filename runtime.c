#include <stdlib.h>
#include <stdio.h>
#include "runtime.h"
#include "object.h"

void init_runtime() {
  // Init extern constants
  TrueObject  = True_new();
  FalseObject = False_new();
  NilObject   = Nil_new();
}

void destroy_runtime() {
  // Init global constants
  free(TrueObject);
  free(FalseObject);
  free(NilObject);
}
