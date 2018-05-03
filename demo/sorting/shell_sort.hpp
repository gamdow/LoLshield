#pragma once

#include "sort_base.hpp"

SortBase::SignedIndex const STEP_SEQUENCE[] = {7, 3, 1, 0};

struct ShellSort : public SortBase {
  void resetMax() {
    __max = N_COLUMNS - 1;
  }

  void resetMin() {
    __min = __max - __sp;
  }

  void resetIndex() {
    __cached = get(__min);
    __idx = __min + __sp;
  }

  virtual void __init() {
    for(__sp_idx = 0; STEP_SEQUENCE[__sp_idx] * 2 > N_COLUMNS - 1; ++__sp_idx);
    __sp = STEP_SEQUENCE[__sp_idx];
    resetMax();
    resetMin();
    resetIndex();
  }

  virtual void __step() {
    if(__idx >= N_COLUMNS || get(__idx) >= __cached) {
      set(__idx - __sp, __cached);
      __min -= __sp;
      if(__min < 0) {
        __max -= 1;
        if(__max < N_COLUMNS - __sp) {
          __sp = STEP_SEQUENCE[++__sp_idx];
          if(__sp < 1) {
            setFinished(true);
            return;
          }
          resetMax();
        }
        resetMin();
      }
      resetIndex();
    } else {
      set(__idx - __sp, get(__idx));
      __idx += __sp;
    }
  }

  int __sp_idx;
  SignedIndex __idx, __min, __max, __sp;
  Element __cached;
};
