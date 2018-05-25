#pragma once

#include "display_params.hpp"

struct DemoBase {
  typedef typename Display::IndexType Index;
  typedef typename Display::ValueType Value;
  static Index const N_ROWS = Display::N_ROWS;
  static Index const N_COLUMNS = Display::N_COLUMNS;
  static Value const MAX_VALUE = Display::MAX_VALUE;

  virtual ~DemoBase() {};

  void update() {__update();}

  void display() {
    for (Index row = 0; row < N_ROWS; ++row) {
      for (Index col = 0; col < N_COLUMNS; ++col) {
        Display::set(col, row, __getValue(col, row));
      }
    }
  }

  float getFrameMS() const {return __getFrameMS();}

private:
  virtual void __update() = 0;
  virtual Value __getValue(Index _col, Index _row) const = 0;
  virtual float __getFrameMS() const = 0;
};
