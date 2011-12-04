#include <stdlib.h>
#include <stdio.h>
#include "runtime.h"
#include "object.h"

void init_runtime() {
  // Init extern constants
  TrueObject  = True_new();
  FalseObject = False_new();
  NilObject   = Nil_new();
  MainObject  = Main_new();
}

void destroy_runtime() {
  // Destroy extern constants
  free(TrueObject);
  free(FalseObject);
  free(NilObject);
  free(MainObject);
}

Object* call(Object *receiver, char *method, Object **argv, int argc) {
  printf("called #%s on ", method);
  Object_print(receiver);
  printf(" with %d arguments.", argc);

  return receiver;
}
