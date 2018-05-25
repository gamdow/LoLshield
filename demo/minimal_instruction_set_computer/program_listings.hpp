#pragma once

#include "instruction_set.hpp"
#include "label.hpp"

namespace MISC {

// fibonacciVisitor
// static Expression const listing[] = {
// START(ND + LOAD), 0,
//   STORE, A,
//   STORE, B,
//   INC, B,
// LOOP(LOAD), A,
//   ADD, B,
//   STORE, C,
//   STORE, D(OUT),
//   INC, D,
//   LOAD, B,
//   STORE, A,
//   LOAD, C,
//   STORE, B,
//   JMP, LOOP,
//   HALT,
// A(0),
// B(0),
// C(0),
// OUT(0)
// };

// primeFactory


Label const A;
Label const B;
Label const C;
Label const D;
Label const OUT;
Label const START;
Label const CONTINUE;
Label const WAIT;
Label const WRITE_ZERO;
Label const RESET_DIV;
Label const NEXT_DIV;
Label const INC_COUNT;
Label const OUT_REF;
Label const OUT_REF_COPY;

static Expression const listing[] = {
START(ND + INC), 1,
  ND + SUB, 32,
  JMP_N, CONTINUE,
  HALT,
WRITE_ZERO(INC), OUT_REF,
  STORE, WAIT - 1,
  STORE, WAIT + 1,
  ND + LOAD, 4,
  STORE, 0,
WAIT(DEC), 0,
  JMP_Z, START,
  JMP, WAIT,
CONTINUE(LOAD), START + 1,
RESET_DIV(STORE), A,
  DEC, A,
  JMP_Z, WRITE_ZERO,
  INC, A,
  ND + LOAD, 1,
  STORE, B,
NEXT_DIV(INC), B,
  LOAD, A,
  STORE, C,
  ND + LOAD, 0,
  STORE, D,
INC_COUNT(INC), D,
  LOAD, C,
  SUB, B,
  JMP_N, NEXT_DIV,
  STORE, C,
  JMP_P, INC_COUNT,
  INC, OUT_REF,
  STORE, OUT_REF_COPY,
  LOAD, B,
  STORE, OUT_REF_COPY(0),
  LOAD, D,
  JMP, RESET_DIV,
A(0),
B(0),
C(0),
D(0),
OUT_REF(OUT),
OUT(0)
};

}
