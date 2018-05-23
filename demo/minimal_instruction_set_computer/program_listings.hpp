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
Label const RESET_DIV;
Label const NEXT_DIV;
Label const INC_COUNT;
Label const OUT_REF;

static Expression const listing[] = {
START(LOAD), 0,
  STORE, A,
  INC, START + 1,
  ND + SUB, A,
  JMP_N, RESET_DIV,
  HALT,
RESET_DIV(LOAD), A,
  DEC, A,
  JMP_Z, START,
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
  LOAD, B,
  STORE, OUT_REF(OUT),
  INC, OUT_REF,
  LOAD, D,
  STORE, A,
  JMP, RESET_DIV,
A(0),
B(0),
C(0),
D(0),
OUT(0)
};

}
