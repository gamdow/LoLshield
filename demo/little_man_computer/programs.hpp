#pragma once

#include "params.hpp"

namespace LMC {

static InstructionType const fibonacciVisitor[] = {
  INBOX,
  COPY_TO, 3,
  SUB_FROM, 3,
  COPY_TO, 0,
  COPY_TO, 1,
  ADD_ONE_TO, 1,
  OUTBOX,
// 12
  COPY_FROM, 1,
  COPY_TO, 2,
  ADD_FROM, 0,
  COPY_TO, 1,
  SUB_FROM, 3,
  JUMP_IF_POS, 0,
  COPY_FROM, 2,
  COPY_TO, 0,
  COPY_FROM, 1,
  OUTBOX,
  JUMP, 12
};







































































static InstructionType const primeFactory[] = {
  COPY_FROM, // init
  0,
  SUB_FROM,
  0,
  COPY_TO,
  0,
  ADD_ONE_TO,
  0,
  COPY_TO,
  1,
  ADD_ONE_TO,
  1,
  COPY_TO,
  2,
  ADD_ONE_TO,
  2,
  ADD_FROM,
  1,
  COPY_TO,
  3,
  ADD_FROM,
  1,
  COPY_TO,
  4,
  SUB_FROM,
  4,
  COPY_TO,
  5,
  INBOX, // start
  COPY_TO,
  9,
  SUB_FROM,
  0,
  JUMP_IF_ZERO,
  28,
  COPY_FROM, // reset factor
  0,
  COPY_TO,
  10,
  COPY_FROM,
  9,
  SUB_FROM,
  PTR + 10,
  JUMP_IF_POS,
  41,
  JUMP_IF_ZERO, // is factor
  58,
  ADD_ONE_TO,
  10,
  COPY_FROM,
  PTR + 10,
  JUMP_IF_POS,
  39, //
  COPY_FROM,
  9,
  OUTBOX,
  JUMP,
  28,
  COPY_FROM,
  PTR + 10,
  OUTBOX,
  COPY_FROM,
  5,
  COPY_TO,
  8,
  ADD_ONE_TO,
  8,
  COPY_FROM,
  9,
  SUB_FROM,
  PTR + 10,
  COPY_TO,
  9,
  JUMP_IF_POS,
  65,
  COPY_FROM,
  8,
  JUMP,
  29,
  HALT
};

// static InstructionType const primeFactory[] = {
//   COPY_FROM, // init
//   4,
//   SUB_FROM,
//   4,
//   COPY_TO,
//   4,
//   COPY_TO,
//   0,
//   ADD_ONE_TO,
//   0,
//   ADD_ONE_TO,
//   0,
//   COPY_TO,
//   1,
//   ADD_ONE_TO,
//   1,
//   ADD_FROM,
//   0,
//   COPY_TO,
//   2,
//   ADD_FROM,
//   0,
//   COPY_TO,
//   3,
//   INBOX, // start
//   COPY_TO,
//   9,
//   COPY_FROM, // reset factor
//   4,
//   COPY_TO,
//   10,
//   COPY_FROM,
//   9,
//   SUB_FROM,
//   PTR + 10,
//   JUMP_IF_POS,
//   33,
//   JUMP_IF_ZERO, // is factor
//   50,
//   ADD_ONE_TO,
//   10,
//   COPY_FROM,
//   PTR + 10,
//   JUMP_IF_POS,
//   31,
//   COPY_FROM,
//   9,
//   OUTBOX,
//   JUMP,
//   24,
//   COPY_FROM,
//   PTR + 10,
//   OUTBOX,
//   COPY_FROM, // divide by factor
//   4,
//   COPY_TO,
//   8,
//   ADD_ONE_TO,
//   8,
//   COPY_FROM,
//   9,
//   SUB_FROM,
//   PTR + 10,
//   COPY_TO,
//   9,
//   JUMP_IF_POS,
//   57,
//   COPY_FROM,
//   8,
//   COPY_TO,
//   9,
//   JUMP,
//   27,
//   HALT
// };

static InstructionType const testPointer[] = {
  COPY_FROM, 0,
  SUB_FROM, 0,
  ADD_ONE_TO, 0,
  COPY_TO, PTR + 0,
  JUMP, 4
};

}
