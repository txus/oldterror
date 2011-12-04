#ifndef __runtime_h__
#define __runtime_h__
#include "object.h"

extern Object *TrueObject;
extern Object *FalseObject;
extern Object *NilObject;

void init_runtime();
void destroy_runtime();

#endif
