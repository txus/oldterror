#include <stdlib.h>
#include <terror/runtime.h>
#include <terror/object.h>
#include <terror/vmmethod.h>
#include <terror/primitives.h>
#include <terror/gc.h>
#include <terror/native_method.h>
#include <terror/dbg.h>
#include <assert.h>

Object *TrueObject = NULL;
Object *FalseObject = NULL;
Object *NilObject = NULL;
Object *Lobby = NULL;

// Bootstrap routines

static inline Object *build_toplevel_object()
{
  Object *object = Object_new_with_parent(Lobby);
  debug("TOPLEVEL OBJECT IS %p", object);

  // Define basic native methods
  bstring name = bfromcstr("clone");
  Object_define_native_method(object, name, Primitive_Object_clone, 0);

  debug("Object.clone IS %p", Object_get_slot(object, name));

  return object;
}

#define DEF_PRIMITIVE(F, N, K, V, A) (F)[(N)] = String_new(bfromcstr(K)); (F)[(N)+1] = Function_native_new(V, A);

static inline Object *build_primitive_object()
{
  Object **fields = calloc(8, sizeof(Object*));
  DEF_PRIMITIVE(fields, 0, "+", Primitive_Integer_add, 1);
  DEF_PRIMITIVE(fields, 2, "-", Primitive_Integer_sub, 1);
  DEF_PRIMITIVE(fields, 4, "*", Primitive_Integer_mul, 1);
  DEF_PRIMITIVE(fields, 6, "/", Primitive_Integer_div, 1);

  Object *array     = Array_new(fields, 8);
  Object *primitive = Hash_new(array);

  Object_destroy(array);
  free(fields);

  debug("VM.primitive IS %p", primitive);

  return primitive;
}

static inline Object *build_vm_object_from(Object *toplevel)
{
  Object *vm = Object_new_with_parent(toplevel);
  debug("VM IS %p", vm);

  Object *primitive = build_primitive_object();
  Object_register_slot(vm, bfromcstr("primitive"), retain(primitive));

  return vm;
}

void Runtime_Lobby_bootstrap()
{
  // Add toplevel object
  Object *toplevel_object = build_toplevel_object();
  Object_register_slot(Lobby, bfromcstr("Object"), retain(toplevel_object));

  // Add native methods
  Object_define_native_method(Lobby, bfromcstr("print"), Primitive_print, 1);
  Object_define_native_method(Lobby, bfromcstr("puts"), Primitive_puts, 1);
  Object_define_native_method(Lobby, bfromcstr("hash"), Primitive_Object_hash, 1);

  // Define VM object
  Object *vm_object = build_vm_object_from(toplevel_object);
  Object_register_slot(Lobby, bfromcstr("VM"), retain(vm_object));
}

void Runtime_init() {
  // Init extern constants
  TrueObject  = True_new();
  FalseObject = False_new();
  NilObject   = Nil_new();
  Lobby       = Lobby_new();
  Runtime_Lobby_bootstrap();
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

Object **extract_args(Object **regs, int from, int arity)
{
  if(arity > 0) {
    Object **argv = calloc(arity, sizeof(Object*));
    int i = 0;

    // From the start register of the arguments (C),
    // get the arguments R(C+0), R(C+1) ... R(C+ARITY)
    for(i=0; i < arity; i++) {
      argv[i] = regs[from + i];
    }

    return argv;
  } else {
    return NULL;
  }
}
