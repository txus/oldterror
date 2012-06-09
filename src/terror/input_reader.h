#ifndef __input_reader_h__
#define __input_reader_h__

#include <terror/instruction.h>
#include <terror/bstrlib.h>

typedef struct {
  bstring     filename;
  bstring     method;

  int         locals_count;
  int         registers_count;

  int         literals_count;
  long        *literals;

  int         instructions_count;
  Instruction **instructions;
} BytecodeFile;

BytecodeFile *BytecodeFile_new(bstring filename);
void BytecodeFile_destroy(BytecodeFile *file);

#endif
