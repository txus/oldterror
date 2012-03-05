#ifndef __instruction_h__
#define __instruction_h__

#include <stdint.h>

typedef struct {
  uint8_t opcode;
  struct {
    uint8_t a;
    uint8_t b;
    uint8_t c;
  } fields;
} Instruction;

Instruction Instruction_new(uint32_t);

#endif
