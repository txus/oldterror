#ifndef __opcode_h__
#define __opcode_h__
enum {
  PUSH_INT,
  PUSH_STRING,
  PUSH_SELF,

  GET_LOCAL,
  SET_LOCAL,

  ADD,
  POP,
  RET,
  DEBUG
};

typedef unsigned char byte;
#endif
