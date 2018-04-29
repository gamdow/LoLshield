#pragma once

#include "sort_base.hpp"

struct InsertionSort : public SortBase {
  virtual void initImpl() {
    idx = 0;
    end_idx = 1;
    sortValue = get(end_idx);
  }

  virtual void stepImpl() {
    if(idx == -1 || get(idx) >= sortValue)
    {
      set(idx + 1, sortValue);
      ++end_idx;
      if(end_idx == N_COLUMNS) {
        setFinished(true);
      } else {
        sortValue = get(end_idx);
        idx = end_idx - 1;
      }
    } else {
      set(idx + 1, get(idx));
      --idx;
    }
  }

  SignedIndex idx, end_idx;
  Element sortValue;
};
