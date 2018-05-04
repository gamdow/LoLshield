#pragma once

#include "../display_params.hpp"

namespace LMC {

typedef int InstructionType;
typedef InstructionType ValueType;

enum Instruction : InstructionType {
  ADDRESS_NONE = 0,
  HALT = ADDRESS_NONE,
  INBOX,
  OUTBOX,
  ADDRESS_MEMORY,
  COPY_FROM = ADDRESS_MEMORY,
  COPY_TO,
  ADD_FROM,
  SUB_FROM,
  ADD_ONE_TO,
  SUB_ONE_TO,
  ADDRESS_PROGRAM,
  JUMP = ADDRESS_PROGRAM,
  JUMP_IF_ZERO,
  JUMP_IF_NEG,
  JUMP_IF_POS
};

enum Value : ValueType {
  PTR = 1 << (Display::N_ROWS - 1)
};

bool hasValue(InstructionType _ins) {
  return _ins >= ADDRESS_MEMORY;
}

bool hasAddressMemory(InstructionType _ins) {
  return _ins >= ADDRESS_MEMORY && _ins < ADDRESS_PROGRAM;
}

bool isPointerValue(ValueType _val) {
  return _val & PTR;
}

ValueType getPointerlessValue(ValueType _val) {
  return _val - (isPointerValue(_val) ? PTR : 0);
}

}
