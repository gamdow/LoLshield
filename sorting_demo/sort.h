#ifndef sort_h
#define sort_h

#include "lolshield.h"

typedef uint8_t element_t;

element_t elements[N_COLUMNS];
float reads[N_COLUMNS];
float writes[N_COLUMNS];
int read_count = 0;
int write_count = 0;

void init_elements() {
  for(byte i = 0; i < N_COLUMNS; ++i ) {
    elements[i] = random(0, N_ROWS);
    reads[i] = 0;
    writes[i] = 0;
    read_count = 0;
    write_count = 0;
  }
}

void decay_hightlights(float decay) {
  for(byte i = 0; i < N_COLUMNS; ++i ) {
    reads[i] *= decay;
    writes[i] *= decay;
    read_count = 0;
    write_count = 0;
  }
}

struct SortBase {
  void SetFinished(bool value) {
    finished = value;
  }

  bool GetFinished() const {
    return finished;
  }

  SortBase() {
    init();
    finished = true;
  }

  virtual void init() {
    init_elements();
    finished = false;
  }

  element_t get(int i) const {
    ++read_count;
    reads[i] = 1.0f;
    return elements[i];
  }

  void set(int i, element_t e) {
    ++write_count;
    writes[i] = 1.0f;
    elements[i] = e;
  }

  virtual void step() = 0;

  bool finished;
};

struct BubbleSortData {
  int8_t min_idx;
  int8_t max_idx;
  int8_t idx;
  int8_t sp;
  int8_t dir;
};

struct BubbleSortBase : public BubbleSortData, public SortBase {
  virtual void init() {
    SortBase::init();
    min_idx = 0;
    max_idx = N_COLUMNS - 1;
    idx = max_idx;
    sp = 1;
    dir = -1;
  }

  void compSwap(int _l, int _r) {
    if(dir * (get(_l) - get(_r)) > 0) {
      element_t t = get(_r);
      set(_r, get(_l));
      set(_l, t);
    }
  }

  virtual void onBoundIdx() {}
  virtual void onBoundBound() {}
  virtual void onBoundDir() {}
  virtual void onBoundSpace() {}

  virtual void step() {
    int n_idx = idx + dir * sp;
    compSwap(idx, n_idx);
    if(n_idx == (dir > 0 ? max_idx : min_idx)) {
      onBoundBound();
      onBoundIdx();
      onBoundDir();
      onBoundSpace();
      if(max_idx - min_idx <= 1 || sp < 1) {
        SetFinished(true);
      }
    } else {
      idx += dir;
    }
  }

  BubbleSortData init_data;
  int8_t min_idx, max_idx, idx, sp, dir;
};

struct BubbleSort : public BubbleSortBase {
  virtual void onBoundIdx() {idx = dir > 0 ? min_idx : max_idx;}
  virtual void onBoundBound() {
    if(dir > 0) {
      max_idx -= 1;
    } else {
      min_idx += 1;
    }
  }
  virtual void onBoundDir() {}
  virtual void onBoundSpace() {}
};

struct CocktailSort : public BubbleSortBase {
  virtual void onBoundIdx() {}
  virtual void onBoundBound() {
    if(dir > 0) {
      max_idx -= 1;
    } else {
      min_idx += 1;
    }
  }
  virtual void onBoundDir() {dir *= -1;}
  virtual void onBoundSpace() {}
};

struct CombSort : public BubbleSortBase {
  virtual void init() {
    BubbleSortBase::init();
    sp = N_COLUMNS - 1;
  }

  virtual void onBoundIdx() {idx = dir > 0 ? min_idx : max_idx;}
  virtual void onBoundBound() {}
  virtual void onBoundDir() {}
  virtual void onBoundSpace() {sp -= 1;}
};

struct SelectionSort : public SortBase {

  virtual void init() {
    SortBase::init();
    start_idx = 0;
    idx = 1;
    min_idx = 0;
  }

  virtual void step() {
    if(idx < N_COLUMNS)
    {
      if(get(idx) < get(min_idx)) {
        min_idx = idx;
      }
      ++idx;
    } else {
      if(min_idx != start_idx) {
        element_t t = get(min_idx);
        set(min_idx, get(start_idx));
        set(start_idx, t);
      }
      ++start_idx;
      min_idx = start_idx;
      idx = start_idx + 1;
      if(idx + 1 == N_COLUMNS) {
        SetFinished(true);
      }
    }
  }

  int8_t idx, start_idx, min_idx;
};

struct InsertionSort : public SortBase {

  virtual void init() {
    SortBase::init();
    idx = 0;
    end_idx = 1;
    sortValue = get(end_idx);
  }

  virtual void step() {
    if(idx == -1 or get(idx) < sortValue)
    {
      set(idx + 1, sortValue);
      ++end_idx;
      if(end_idx == N_COLUMNS) {
        SetFinished(true);
      } else {
        sortValue = get(end_idx);
        idx = end_idx - 1;
      }
    } else {
      set(idx + 1, get(idx));
      --idx;
    }
  }

  int8_t idx, end_idx;
  element_t sortValue;
};

int shell_sort_sq[] = {701, 301, 132, 57, 23, 10, 4, 1};

struct ShellSort : public BubbleSortBase {
  virtual void init() {
    BubbleSortBase::init();
    space_idx = 0;
    while(shell_sort_sq[space_idx] > N_COLUMNS - 1) {
      ++space_idx;
    }
    sp = shell_sort_sq[space_idx];
  }

  virtual void onBoundIdx() {idx = dir > 0 ? min_idx : max_idx;}
  virtual void onBoundBound() {}
  virtual void onBoundDir() {}
  virtual void onBoundSpace() {sp = shell_sort_sq[++space_idx];}

  int space_idx;
};

#endif
