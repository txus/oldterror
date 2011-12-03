#include <stdio.h>
#include <assert.h>
#include "dbg.h"
#include "opcode.h"

int main(int argc, char const *argv[])
{
  byte instructions[] = {
    PUSH_INT, 2,
    PUSH_INT, 2,
    ADD,
    RET
  };

  // Instruction pointer
  byte *ip = instructions;
  int count = sizeof(instructions) / sizeof(byte);
  int i;

  // Print all the instructions
  for(i = 0; i < count; i++) {
    printf("%i", *(ip + i));
  }

  return 0;
}
