#include <stdlib.h>
#include <terror/runtime.h>
#include <terror/object.h>
#include <terror/vmmethod.h>
#include <terror/dbg.h>

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
  TrueObject = NULL;

  free(FalseObject);
  FalseObject = NULL;

  free(NilObject);
  NilObject = NULL;

  free(MainObject);
  MainObject = NULL;
}

static inline Object* call_kernel_method(bstring method, Object **argv) {
  /*
   * #print
   */

  bstring print = bfromcstr("print");

  if (bstrcmp(method, print)==0) {
    if (!argv[0]) {
      printf("[#print] Wrong number of arguments: 0 for 1\n");
      exit(1);
    }
    switch(argv[0]->type) {
      case tString:
        printf("%s", bdata(argv[0]->value.string));
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

  printf("Could not find method %s on ", bdata(method));
  Object_print(MainObject);
  printf("\n");
  exit(1);
}

Object* call_method(Object *receiver, bstring method, Object **argv, int argc, int registers_count) {
  debug("Entering #%s", bdata(method));

  if (receiver == MainObject) {
    Object *result = call_kernel_method(method, argv);
    return result;
  }

  VMMethod *vmmethod = NULL;
  vmmethod = Object_lookup_method(receiver, method);

  debug("Ok, now trying to get it, from outside: %p", vmmethod);
  /* debug("Method has arity %i\\n", vmmethod->arity); */

  if (!vmmethod) {
    // Native global methods
    printf("Could not find method %s on ", bdata(method));
    Object_print(receiver);
    printf("\n");
    exit(1);
  }

  Object *result = VMMethod_execute(vmmethod, argv, argc, registers_count, receiver);

  debug("Returning from #%s", bdata(method));

  return result;
}

