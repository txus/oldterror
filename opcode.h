#ifndef __opcode_h__
#define __opcode_h__
enum {
  PUSH_INT,
  PUSH_STRING,
  ADD,
  POP,
  RET,
  DEBUG
};

typedef unsigned char byte;
#endif
