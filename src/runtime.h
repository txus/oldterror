#ifndef __runtime_h__
#define __runtime_h__
#include "object.h"
#include "vmmethod.h"

extern Object *TrueObject;
extern Object *FalseObject;
extern Object *NilObject;
extern Object *MainObject;

void init_runtime();
void destroy_runtime();

Object* call(Object *receiver, char *method, Object **argv, int argc);
Object* call_kernel_method(char *method, Object **argv, int argc);

#endif
