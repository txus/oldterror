#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "input_reader.h"
#include "util.h"

char**
read_file(const char *filename)
{
  FILE* file = fopen(filename, "rt");

  char **buf = malloc(sizeof(char*)*100);
  int size = 100;
  int i = 0;

  while(!feof(file)) {
    buf[i] = malloc(sizeof(char)*20);
    fgets(buf[i], size, file);
    i++;
  }

  fclose(file);

  return buf;
}

BytecodeFile*
BytecodeFile_new(const char* filename, char* method, int num_registers, int num_locals, int num_literals, char literals[200][200], int num_instructions, char instructions[200][200])
{
  BytecodeFile *bc = malloc(sizeof(BytecodeFile));
  int i = 0;

  bc->filename        = strdup(filename);
  bc->method          = strdup(method);
  bc->locals_count    = num_locals;
  bc->registers_count = num_registers;

  // LITERALS
  bc->literals_count = num_literals;
  long *real_literals = malloc(sizeof(long)*num_literals);
  for(i=0; i < num_literals; i++) {
    if (strncmp(literals[i], "\"", 1) == 0) { // It's a string literal
      int to = strlen(literals[i]) - 1;
      real_literals[i]  = (long)(strndup(literals[i]+1, to));
    } else { // It's a numeric literal
      real_literals[i]  = (long)(atoi(literals[i]));
    }
  }
  bc->literals = real_literals;

  // INSTRUCTIONS
  bc->instructions_count = num_instructions;
  Instruction *real_instructions[num_instructions];
  for(i=0; i < num_instructions; i++) {
    uint32_t num;
    sscanf(instructions[i], "%x", &num);
    real_instructions[i] = Instruction_new(num);
  }
  bc->instructions = real_instructions;

  return bc;
}

BytecodeFile*
load_file(const char* filename)
{
  char **buf = read_file(filename);

  int num_regs, num_locals, num_literals, num_instructions;
  char literals[200][200];
  char instructions[200][200];
  char method[20];

  char **ptr = buf;
  while(*ptr) {
    char *line = *ptr;
    if (strncmp(line, "_", 1) == 0) { // VMMETHOD HEADER
      printf("HEADER\n");
      sscanf (line, "_%s", method);
    } else if (strncmp(*ptr, ":", 1) == 0) { // LITERALS POOL
      sscanf(line, ":%i:%i:%i:%i", &num_regs, &num_locals, &num_literals, &num_instructions);

      printf("Registers: %i\n", num_regs);
      printf("Locals: %i\n", num_locals);
      printf("Literals: %i\n", num_literals);
      printf("Instructions: %i\n", num_instructions);

      int i = 0;
      for(i = 0; i < num_literals; i++) {
        ptr++;
        line = *ptr;
        sscanf(line, "%[^\n]", literals[i]);
      }

      printf("Literal table:\n");
      for(i = 0; i < num_literals; i++) {
        printf("\t%i - %s\n", i, (char*)literals[i]);
      }

    } else if (strncmp(line, "0x", 2) == 0) { // INSTRUCTION

      int i = 0;
      for(i = 0; i < num_instructions; i++) {
        line = *ptr;
        sscanf(line, "%s", instructions[i]);
        ptr++;
      }

      printf("Instructions:\n");
      for(i = 0; i < num_instructions; i++) {
        printf("\t%i - %s\n", i, instructions[i]);
        // decoded peek
        int num;
        sscanf(instructions[i], "%x", &num);
        Instruction *instr = Instruction_new((uint32_t)num);
        printf("\t\tOP[%0x] A[%i] B[%i] C[%i]\n", instr->opcode, instr->fields.a, instr->fields.b, instr->fields.c);
      }
    }
    ptr++;
  }
  
  BytecodeFile *bc_file = BytecodeFile_new(filename, method, num_regs, num_locals, num_literals, literals, num_instructions, instructions);

  return bc_file;
}
