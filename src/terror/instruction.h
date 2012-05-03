#ifndef _terror__instruction_h_
#define _terror__instruction_h_

#include <stdint.h>

typedef struct {
  uint8_t opcode;
  struct {
    uint8_t a;
    uint8_t b;
    uint8_t c;
  } fields;
} Instruction;

Instruction* Instruction_new(uint32_t value);
void Instruction_destroy(Instruction *instruction);

#endif
