#include <stdio.h>
#include <terror/registers.h>

void Registers_print(Object **registers, int registers_count)
{
  int i = 0;

  printf("Register dump: \n");

  for(i=0; i < registers_count; i++) {
    printf("\t[%i]: ", i);
    if (registers[i] != NULL) {
      Object_print(registers[i]);
    } else {
      printf("(empty)");
    }
    printf("\n");
  }
}

void
Registers_cleanup(Object **registers, int registers_count, int except)
{
  int i=0;

  for(i=0; i < registers_count; i++) {
    if (i != except) {
      if(registers[i]) release(registers[i]);
    }
  }
}
