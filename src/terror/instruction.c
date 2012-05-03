#include <stdlib.h>
#include <terror/dbg.h>
#include <terror/instruction.h>

Instruction* Instruction_new(uint32_t value)
{
  Instruction *instruction = calloc(1, sizeof(Instruction));
  check_mem(instruction);

  instruction->opcode   = (value & 0xff000000) >> 24;
  instruction->fields.a = (value & 0x00ff0000) >> 16;
  instruction->fields.b = (value & 0x0000ff00) >> 8;
  instruction->fields.c = value & 0x000000ff;

  return instruction;

error:
  return NULL;
}

void Instruction_destroy(Instruction *instruction)
{
  free(instruction);
}

