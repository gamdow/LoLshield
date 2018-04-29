#pragma once

#include <Arduino.h>
#include <Charliplexing.h>

struct DemoParams {
  typedef uint8_t IndexType;
  typedef uint8_t ValueType;
  static IndexType const N_COLUMNS = 14;
  static IndexType const N_ROWS = 9;
  static ValueType const MAX_VALUE = 7;
  static void set(IndexType col, IndexType row, ValueType val) {
    LedSign::Set(col, row, val);
  }
};
