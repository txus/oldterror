#ifndef __input_reader_h__
#define __input_reader_h__

#include "instruction.h"

typedef struct {
  char        *filename;
  char        *method;

  int         locals_count;
  int         registers_count;

  int         literals_count;
  long        *literals;

  int         instructions_count;
  Instruction **instructions;
} BytecodeFile;

BytecodeFile *load_file(const char *filename);

#endif
