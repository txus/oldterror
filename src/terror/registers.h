#ifndef _terror__registers_h_
#define _terror__registers_h_

#include <terror/object.h>
#include <terror/gc.h>

#define REGISTER(R, V) { Object *obj = (V); retain(obj); (R) = obj; }
#define CLEAN_REGISTER(R) { if (R) release(R); }

void Registers_print(Object **registers, int registers_count);
void Registers_cleanup(Object **registers, int registers_count, int except);

#endif
