#pragma once

#include "expression.hpp"

#define CHAR_BIT 8

namespace MISC {
#ifdef DEC
#undef DEC
#endif

// X Macros
#define INSTRUCTION_TABLE \
  X(HALT, "HALT") \
  Y(ACCUMULATOR_BEGIN) \
  X(LOAD = ACCUMULATOR_BEGIN, "LOAD") \
  X(STORE, "STORE") \
  X(ADD, "ADD") \
  X(SUB, "SUB") \
  X(INC, "INC") \
  X(DEC, "DEC") \
  Y(ACCUMULATOR_END) \
  X(COUNT = ACCUMULATOR_END, "COUNT") \
  Y(NODREF_START) \
  X(JMP = NODREF_START, "JMP") \
  X(JMP_Z, "JMP_Z") \
  X(JMP_N, "JMP_N") \
  X(JMP_P, "JMP_P")

#define X(a, b) a,
#define Y(a) a,
enum : int {
  INSTRUCTION_TABLE
  NUM_INSTRUCTIONS
};
#undef Y
#undef X

inline int ceilPowerTwo(int _v) {
  for(size_t i = sizeof(int) * CHAR_BIT - 1; i > 0; --i) {
    if((_v - 1) >> i) return 1 << (i + 1);
  }
  return 1 << 1;
}

static int const NODREF_MOD = ceilPowerTwo(NUM_INSTRUCTIONS);
static int const ND = NODREF_MOD;

inline bool isAccumulatorInstruction(int _i) {return _i >= ACCUMULATOR_BEGIN && _i < ACCUMULATOR_END;}
inline bool isNodref(int _i) {return _i >= NODREF_START;}
inline bool isNodrefMod(int _i) {return _i >= NODREF_MOD;}

#define X(a, b) b,
#define Y(a)
char const * const INSTRUCTION_NAMES[] = {
  INSTRUCTION_TABLE
};
#undef Y
#undef X

}
