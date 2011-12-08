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
  SUB,
  MUL,
  DIV,

  POP,
  CALL,
  JUMP_UNLESS,
  RET,
  DEBUG,
  DEBUG_TOS
};

typedef unsigned char byte;
#endif
