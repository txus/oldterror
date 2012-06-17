#ifndef _terror__runtime_h_
#define _terror__runtime_h_

#include <terror/object.h>

extern Object *TrueObject;
extern Object *FalseObject;
extern Object *NilObject;
extern Object *Lobby;

void Runtime_init();
void Runtime_Lobby_bootstrap();
void Runtime_destroy();

Object* call_method(Object *receiver, bstring method, Object **argv, int argc, int registers_count);
Object** extract_args(Object **regs, int from, int arity);
Object* Function_call(Object *fn, Object *receiver, Object **argv, int argc, int registers_count);

#endif
