#include <stdio.h>
#include "registers.h"
#include "gc.h"

void
Registers_print(Object **registers)
{
  int i = 0;

  printf("Register dump: \n");

  for(i=0; i < NUM_REGISTERS; i++) {
    printf("\t[%i]: ", i);
    if (registers[i]) {
      Object_print(registers[i]);
    } else {
      printf("(empty)");
    }
    printf("\n");
  }
}

void
Registers_cleanup(Object **registers)
{
  int i=1;

  // Don't wipe the register 0
  for(i=1; i < NUM_REGISTERS; i++) {
    if (registers[i] && registers[i]->type) release(registers[i]);
  }
}
