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

  // Branching
  JMP = 0x20,
  JIF, // Jump if false
  JIT, // Jimp if true

  // Object operations
  LOADSELF = 0x30,

  // Local variables
  LOADLOCAL = 0x50,
  SETLOCAL,

  SEND = 0x80,

  RET = 0x90,
  DUMP,

} OpCode;

#define __OP__(O, A, B, C) 0x0 + (O << 24) + (A << 16) + (B << 8) + C

#define OP_MOVE(A,B)      __OP__(MOVE, A, B, 0)
#define OP_LOADI(A,B)     __OP__(LOADI, A, B, 0)
#define OP_LOADS(A,B)     __OP__(LOADS, A, B, 0)

#define OP_LOADNIL(A)     __OP__(LOADNIL, A, 0, 0)
#define OP_LOADBOOL(A,B)  __OP__(LOADBOOL, A, B, 0)

#define OP_ADD(A,B,C)     __OP__(ADD, A, B, C)
#define OP_SUB(A,B,C)     __OP__(SUB, A, B, C)
#define OP_MUL(A,B,C)     __OP__(MUL, A, B, C)
#define OP_DIV(A,B,C)     __OP__(DIV, A, B, C)

#define OP_JMP(A)         __OP__(JMP, A, 0, 0)
#define OP_JIF(A,B)       __OP__(JIF, A, B, 0)
#define OP_JIT(A,B)       __OP__(JIT, A, B, 0)

#define OP_LOADSELF(A)    __OP__(LOADSELF, A, 0, 0)

#define OP_LOADLOCAL(A,B) __OP__(LOADLOCAL, A, B, 0)
#define OP_SETLOCAL(A,B)  __OP__(SETLOCAL, A, B, 0)

#define OP_SEND(A,B,C)    __OP__(SEND, A, B, C)
#define OP_RET(A)         __OP__(RET, A, 0, 0)
#define OP_DUMP()         __OP__(DUMP, 0, 0, 0)

#endif
