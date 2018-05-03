#pragma once

#include "demo_params.hpp"

struct DemoBase {
  typedef typename DemoParams::IndexType Index;
  typedef typename DemoParams::ValueType Value;
  static Index const N_ROWS = DemoParams::N_ROWS;
  static Index const N_COLUMNS = DemoParams::N_COLUMNS;
  static Value const MAX_VALUE = DemoParams::MAX_VALUE;

  void update() {__update();}

  void display() {
    for (Index row = 0; row < N_ROWS; ++row) {
      for (Index col = 0; col < N_COLUMNS; ++col) {
        DemoParams::set(col, row, __getValue(col, row));
      }
    }
  }

  float getFrameMS() const {return __getFrameMS();}

private:
  virtual void __update() = 0;
  virtual Value __getValue(Index _col, Index _row) const = 0;
  virtual float __getFrameMS() const = 0;
};
