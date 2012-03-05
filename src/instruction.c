#include <stdint.h>
#include "instruction.h"

Instruction
Instruction_new(uint32_t encoded)
{
  Instruction instruction = {
    .opcode = (encoded & 0xff000000) >> 24,
    .fields = {
      .a = (encoded & 0x00ff0000) >> 16,
      .b = (encoded & 0x0000ff00) >> 8,
      .c = encoded & 0x000000ff
    }
  };

  return instruction;
}
