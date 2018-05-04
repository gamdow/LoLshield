#pragma once

#include <Arduino.h>
#include <Charliplexing.h>

namespace Display {

typedef uint8_t IndexType;
typedef uint8_t ValueType;
IndexType const N_COLUMNS = 14;
IndexType const N_ROWS = 9;
ValueType const MAX_VALUE = 7;

void set(IndexType _col, IndexType _row, ValueType _val) {
  LedSign::Set(_col, _row, _val);
}

}
