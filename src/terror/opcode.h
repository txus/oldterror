#ifndef _terror__opcode_h_
#define _terror__opcode_h_

typedef enum {
  NOOP,
  MOVE,
  LOADI,
  LOADS,

  LOADNIL = 0x08,
  LOADBOOL,

  // Branching
  JMP = 0x20,
  JIF, // Jump if false
  JIT, // Jimp if true

  // Object operations
  LOADSELF = 0x30,

  // Local variables
  LOADLOCAL = 0x50,
  SETLOCAL,

  LOADSLOT = 0x60,
  SETSLOT,

  MAKEARRAY = 0x70,

  SEND = 0x80,

  RET = 0x90,
  DUMP,

} OpCode;

#define __OP__(O, A, B, C) 0x0 + (O << 24) + (A << 16) + (B << 8) + C

#define OP_NOOP()         __OP__(NOOP, 0, 0, 0)
#define OP_MOVE(A,B)      __OP__(MOVE, A, B, 0)
#define OP_LOADI(A,B)     __OP__(LOADI, A, B, 0)
#define OP_LOADS(A,B)     __OP__(LOADS, A, B, 0)

#define OP_LOADNIL(A)     __OP__(LOADNIL, A, 0, 0)
#define OP_LOADBOOL(A,B)  __OP__(LOADBOOL, A, B, 0)

#define OP_JMP(A)         __OP__(JMP, A, 0, 0)
#define OP_JIF(A,B)       __OP__(JIF, A, B, 0)
#define OP_JIT(A,B)       __OP__(JIT, A, B, 0)

#define OP_LOADSELF(A)    __OP__(LOADSELF, A, 0, 0)

#define OP_LOADLOCAL(A,B) __OP__(LOADLOCAL, A, B, 0)
#define OP_SETLOCAL(A,B)  __OP__(SETLOCAL, A, B, 0)

#define OP_LOADSLOT(A,B,C) __OP__(LOADSLOT, A, B, C)
#define OP_SETSLOT(A,B,C)  __OP__(SETSLOT, A, B, C)

#define OP_MAKEARRAY(A,B,C)  __OP__(MAKEARRAY, A, B, C)

#define OP_SEND(A,B,C)    __OP__(SEND, A, B, C)
#define OP_RET(A)         __OP__(RET, A, 0, 0)
#define OP_DUMP()         __OP__(DUMP, 0, 0, 0)

#endif
