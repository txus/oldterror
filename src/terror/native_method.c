#include <terror/native_method.h>

NativeMethod*
NativeMethod_new(native_fn fn, short arity)
{
  NativeMethod *method = calloc(1, sizeof(NativeMethod));

  method->fn    = fn;
  method->arity = arity;

  return method;
}

Object*
NativeMethod_execute(NativeMethod *method, Object **locals, int locals_count, int _, Object *self)
{
  native_fn fn = method->fn;
  switch(method->arity) {
  case 0:
    return fn(self, NULL, NULL);
  case 1:
    return fn(self, locals[0], NULL);
  case 2:
    return fn(self, locals[0], locals[1]);
  }
  return NULL;
}
