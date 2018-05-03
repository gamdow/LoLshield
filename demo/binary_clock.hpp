#pragma once

#include "demo_base.hpp"

struct BinaryClock : public DemoBase {
  static long const MAX_COUNT = 1L << N_COLUMNS;
  static float const FRAME_TIME_MS = static_cast<float>(60 * 60 * 1000L / MAX_COUNT);

  BinaryClock()
    : __count(0)
  {
  }

private:
  virtual void __update() {
    __count += 1L;
    if (__count > MAX_COUNT) {
      __count = 0L;
    }
  }

  virtual Value __getValue(Index _col, Index _row) const {
    if ((__count - _row) & (1 << _col)) {
      return MAX_VALUE - (_row * MAX_VALUE) / N_ROWS;
    } else {
      return 0;
    }
  }

  virtual float __getFrameMS() const {return FRAME_TIME_MS;}

  long __count;
};
