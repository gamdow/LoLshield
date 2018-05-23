#pragma once

#include "sort_base.hpp"

struct InsertionSort : public SortBase {
  InsertionSort(SortData & _data) : SortBase(_data) {}
  
private:
  void resetIndex() {
    __cached = get(__min);
    __idx = __min + 1;
  }

  virtual void __init() {
    __min = N_COLUMNS - 2;
    resetIndex();
  }

  virtual void __step() {
    if(__idx >= N_COLUMNS || get(__idx) >= __cached)
    {
      set(__idx - 1, __cached);
      --__min;
      if(__min < 0) {
        setFinished(true);
        return;
      }
      resetIndex();
    } else {
      set(__idx - 1, get(__idx));
      ++__idx;
    }
  }

  SignedIndex __idx, __min;
  Element __cached;
};
