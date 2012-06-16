#ifndef _terror__native_method_h_
#define _terror__native_method_h_

#include <terror/object.h>

typedef struct {
  native_fn fn; // from object.h
  short arity;
} NativeMethod;

NativeMethod* NativeMethod_new(native_fn fn, short arity);
#define NativeMethod_destroy(A) free(A)
Object *NativeMethod_execute(NativeMethod *method, Object **locals, int locals_count, int _, Object *self);

#endif
