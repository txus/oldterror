#ifndef __opcode_h__
#define __opcode_h__

typedef enum {
  MOVE,
  LOADI,
  LOADS,

  LOADNIL = 0x08,
  LOADBOOL,

  // Numerical operations.
  //  Each instruction takes three registers.
  ADD = 0x10,
  SUB,
  MUL,
  DIV,

  // Object operations
  LOADSELF = 0x30,

  // Local variables
  LOAD_LOCAL = 0x50,
  SET_LOCAL,

  RET = 0x90,
  DUMP,

} OpCode;

#endif
