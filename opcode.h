#ifndef __opcode_h__
#define __opcode_h__
enum {
  PUSH_INT,
  PUSH_STRING,
  PUSH_SELF,

  PUSH_BOOL,
  PUSH_NIL,

  GET_LOCAL,
  SET_LOCAL,

  ADD,
  POP,
  CALL,
  JUMP_UNLESS,
  RET,
  DEBUG
};

typedef unsigned char byte;
#endif
