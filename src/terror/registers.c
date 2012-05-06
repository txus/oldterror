#include <terror/registers.h>

void Registers_print(Object **registers, int len)
{
  int i = 0;

  printf("Register dump: \n");

  for(i=0; i < len; i++) {
    printf("\t[%i]: ", i);
    if (registers[i] != NULL) {
      Object_print(registers[i]);
    } else {
      printf("(empty)");
    }
    printf("\n");
  }
}
