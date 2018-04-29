#pragma once

#include "sort_base.hpp"

struct SelectionSort : public SortBase {
  virtual void initImpl() {
    start_idx = 0;
    idx = 1;
    min_idx = 0;
  }

  virtual void stepImpl() {
    if(idx < N_COLUMNS)
    {
      if(get(idx) > get(min_idx)) {
        min_idx = idx;
      }
      ++idx;
    } else {
      if(min_idx != start_idx) {
        Element t = get(min_idx);
        set(min_idx, get(start_idx));
        set(start_idx, t);
      }
      ++start_idx;
      min_idx = start_idx;
      idx = start_idx + 1;
      if(idx == N_COLUMNS) {
        setFinished(true);
      }
    }
  }

  SignedIndex idx, start_idx, min_idx;
};
