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

Object* call_method(Object *receiver, bstring method, Object **argv, int argc, int registers_count) {
  debug("Entering #%s", bdata(method));

  Object *fn = Object_lookup_slot(receiver, method);
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
