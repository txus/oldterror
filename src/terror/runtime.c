#include <stdlib.h>
#include <terror/runtime.h>

Object *TrueObject = NULL;
Object *FalseObject = NULL;
Object *NilObject = NULL;
Object *MainObject = NULL;

void Runtime_init() {
  // Init extern constants
  TrueObject  = True_new();
  FalseObject = False_new();
  NilObject   = Nil_new();
  MainObject  = Main_new();
}

void Runtime_destroy() {
  free(TrueObject);
  free(FalseObject);
  free(NilObject);
  free(MainObject);
}
