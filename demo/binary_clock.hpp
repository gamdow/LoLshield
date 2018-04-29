#pragma once

#include "demo_base.hpp"

struct BinaryClock : public DemoBase {
  static long const MAX_COUNT = 1L << N_COLUMNS;
  static float const FRAME_TIME = static_cast<float>(60 * 60 * 1000L / MAX_COUNT);

  BinaryClock()
    : __count(0)
  {
  }

private:
  virtual void updateImpl() {
    __count += 1L;
    if (__count > MAX_COUNT) {
      __count = 0L;
    }
  }

  virtual Value getValue(Index col, Index row) const {
    if ((__count - row) & (1 << col)) {
      return MAX_VALUE - (row * MAX_VALUE) / N_ROWS;
    } else {
      return 0;
    }
  }

  virtual float getFrameMSImpl() const {return FRAME_TIME;}

  long __count;
};
