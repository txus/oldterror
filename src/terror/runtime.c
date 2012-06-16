#include <stdlib.h>
#include <terror/runtime.h>
#include <terror/object.h>
#include <terror/vmmethod.h>
#include <terror/native_method.h>
#include <terror/dbg.h>
#include <assert.h>

Object *TrueObject = NULL;
Object *FalseObject = NULL;
Object *NilObject = NULL;
Object *Lobby = NULL;

void Runtime_init() {
  // Init extern constants
  TrueObject  = True_new();
  FalseObject = False_new();
  NilObject   = Nil_new();
  Lobby       = Lobby_new();
}

void Runtime_destroy() {
  Object_destroy_immortal(TrueObject);
  TrueObject = NULL;

  Object_destroy_immortal(FalseObject);
  FalseObject = NULL;

  Object_destroy_immortal(NilObject);
  NilObject = NULL;

  Object_destroy_immortal(Lobby);
  Lobby = NULL;
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
      case tFunction:
        printf("#<tFunction:%p @method=\"%p\">", argv[0], argv[0]->value.other);
        break;
      case tArray:
        printf("#<tArray:%p @contents=[", argv[0]);
        DArray *array = (DArray*)argv[0]->value.other;

        int i = 0, count = DArray_count(array);
        for(i=0; i < count; i++) {
          Object_print((Object*)DArray_at(array, i));
          if (i+1 != count) printf(", ");
        }

        printf("]>");
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
  Object_print(Lobby);
  printf("\n");
  exit(1);
}

Object* call_method(Object *receiver, bstring method, Object **argv, int argc, int registers_count) {
  debug("Entering #%s", bdata(method));

  if (receiver == Lobby) {
    Object *result = call_kernel_method(method, argv);
    return result;
  }

  Object *fn = Object_get_slot(receiver, method);
  if (fn == NULL) {
    printf("Could not find method %s on ", bdata(method));
    Object_print(receiver);
    printf("\n");
    exit(1);
  }
  assert(fn->type == tFunction && "Tried to call a non-function");

  Object *result = Function_call(fn, receiver, argv, argc, registers_count);

  debug("Returning from #%s", bdata(method));

  return result;
}

Object* Function_call(Object *fn, Object *receiver, Object **argv, int argc, int registers_count) {
  if(fn->native) {
    NativeMethod *method = (NativeMethod*)fn->value.other;
    return NativeMethod_execute(method, argv, argc, registers_count, receiver);
  } else {
    VMMethod *method = (VMMethod*)fn->value.other;
    return VMMethod_execute(method, argv, argc, registers_count, receiver);
  }
}
