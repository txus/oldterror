#ifndef _terror__runtime_h_
#define _terror__runtime_h_

#include <terror/object.h>

extern Object *TrueObject;
extern Object *FalseObject;
extern Object *NilObject;
extern Object *MainObject;

void Runtime_init();
void Runtime_destroy();

#endif
