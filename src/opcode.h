#ifndef __opcode_h__
#define __opcode_h__

typedef enum {
  MOVE,
  LOADI,

  // Numerical operations.
  //  Each instruction takes three registers.
  ADD = 0x10,
  SUB,

  RET = 0x90,
  DUMP,

} OpCode;

#endif
