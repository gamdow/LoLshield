#pragma once

#include "sort_base.hpp"

struct SelectionSort : public SortBase {
  SelectionSort(SortData & _data) : SortBase(_data) {}

private:
  void resetIndex() {
    __max_val = __tar;
    __idx = 0;
  }

  virtual void __init() {
    __tar = N_COLUMNS - 1;
    resetIndex();
  }

  virtual void __step() {
    if(__idx < __tar)
    {
      if(get(__idx) > get(__max_val)) {
        __max_val = __idx;
      }
      ++__idx;
    } else {
      if(__max_val != __tar) {
        Element t = get(__max_val);
        set(__max_val, get(__tar));
        set(__tar, t);
      }
      --__tar;
      resetIndex();
      if(__idx == __tar) {
        setFinished(true);
      }
    }
  }

  SignedIndex __idx, __tar, __max_val;
};
