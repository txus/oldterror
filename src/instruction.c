#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "instruction.h"

Instruction*
Instruction_new(uint32_t encoded)
{
  Instruction *allocated = malloc(sizeof(Instruction));

  Instruction instruction = {
    .opcode = (encoded & 0xff000000) >> 24,
    .fields = {
      .a = (encoded & 0x00ff0000) >> 16,
      .b = (encoded & 0x0000ff00) >> 8,
      .c = encoded & 0x000000ff
    }
  };

  memcpy(allocated, &instruction, sizeof instruction);

  return allocated;
}
