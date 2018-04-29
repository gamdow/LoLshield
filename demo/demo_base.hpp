#pragma once

#include "demo_params.hpp"

struct DemoBase {
  typedef typename DemoParams::IndexType Index;
  typedef typename DemoParams::ValueType Value;
  static Index const N_ROWS = DemoParams::N_ROWS;
  static Index const N_COLUMNS = DemoParams::N_COLUMNS;
  static Value const MAX_VALUE = DemoParams::MAX_VALUE;

  void update() {updateImpl();}

  void display() {
    for (Index row = 0; row < N_ROWS; ++row) {
      for (Index col = 0; col < N_COLUMNS; ++col) {
        DemoParams::set(col, row, getValue(col, row));
      }
    }
  }

  float getFrameMS() const {return getFrameMSImpl();}

private:
  virtual void updateImpl() = 0;
  virtual Value getValue(Index col, Index row) const = 0;
  virtual float getFrameMSImpl() const = 0;
};
