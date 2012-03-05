#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "runtime.h"
#include "object.h"
#include "vmmethod.h"
#include "registers.h"
#include "dbg.h"

Object* VMMethod_execute(VMMethod *method, Object **locals, Object *self);

void init_runtime() {
  // Init extern constants
  TrueObject  = True_new();
  FalseObject = False_new();
  NilObject   = Nil_new();
  MainObject  = Main_new();
}

void destroy_runtime() {
  free(TrueObject);
  free(FalseObject);
  free(NilObject);
  free(MainObject);
}

Object* call(Object *receiver, const char *method, Object **argv, int argc) {
  VMMethod *vmmethod = Object_lookup_method(receiver, method);

  debug("Entering #%s", method);

  if (!vmmethod) {
    // Native global methods
    if (receiver == MainObject) {
      Object *result = call_kernel_method(method, argv, argc);
      return result;
    } else {
      printf("Could not find method %s on ", method);
      Object_print(receiver);
      printf("\n");
      exit(1);
    }
  }

  Object *result = VMMethod_execute(vmmethod, argv, receiver);

  debug("Returning from #%s", method);

  return result;
}

Object* call_kernel_method(const char *method, Object **argv, int argc) {
  /*
   * #print
   */

  if (strcmp(method, "print")==0) {
    if (!argv[0]) {
      printf("[#print] Wrong number of arguments: 0 for 1\n");
      exit(1);
    }
    switch(argv[0]->type) {
      case tString:
        printf("%s", argv[0]->value.string);
        break;
      case tInteger:
        printf("%i", argv[0]->value.integer);
        break;
      case tTrue:
        printf("true");
        break;
      case tFalse:
        printf("false");
        break;
      case tNil:
        printf("nil");
        break;
      case tObject:
        Object_print(argv[0]);
        printf("%i", argv[0]->value.integer);
        break;
    }
    return NilObject;
  }

  /*
   * #. . .
   */


  /*
   * RAISE!
   */

  printf("Could not find method %s on ", method);
  Object_print(MainObject);
  printf("\n");
  exit(1);
}
