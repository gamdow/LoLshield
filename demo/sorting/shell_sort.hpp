#pragma once

#include "sort_base.hpp"

int shell_sort_sq[] = {7, 3, 1, 0};

struct ShellSort : public SortBase {
  bool updateIdx() {
    if(end_idx + sp < N_COLUMNS) {
      idx = end_idx;
      end_idx += sp;
    } else {
      if(start_idx + 1 < sp) {
        start_idx += 1;
      } else if(sp > 1) {
        sp = shell_sort_sq[++space_idx];
        start_idx = 0;
      } else {
        return false;
      }
      idx = start_idx;
      end_idx = start_idx + sp;
    }
    sortValue = get(end_idx);
    return true;
  }

  virtual void initImpl() {
    space_idx = 0;
    while(shell_sort_sq[space_idx] * 2 > N_COLUMNS - 1) {
      ++space_idx;
    }
    sp = shell_sort_sq[space_idx];
    start_idx = 0;
    idx = 0;
    end_idx = sp;
    sortValue = get(end_idx);
  }

  virtual void stepImpl() {
    if(idx < start_idx || get(idx) >= sortValue) {
      set(idx + sp, sortValue);
      bool cont = updateIdx();
      if(!cont) {
        setFinished(true);
      }
    } else {
      set(idx + sp, get(idx));
      idx -= sp;
    }
  }

  SignedIndex idx, start_idx, end_idx, space_idx, sp;
  Element sortValue;
};
