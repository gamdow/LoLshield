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

struct BubbleSort : public SortBase {

  virtual void init() {
    SortBase::init();
    idx = 0;
    max_idx = N_COLUMNS - 1;
  }

  virtual void step() {
    if(get(idx) > get(idx + 1)) {
      element_t t = get(idx + 1);
      set(idx + 1, get(idx));
      set(idx, t);
    }

    if(idx + 1 == max_idx) {
      idx = 0;
      max_idx -= 1;
      if(max_idx <= 1) {
        SetFinished(true);
      }
    } else {
      ++idx;
    }
  }

  int8_t idx, max_idx;
};

struct CombSort : public SortBase {

  virtual void init() {
    SortBase::init();
    idx = 0;
    sp = N_COLUMNS - 1;
  }

  virtual void step() {
    if(get(idx) > get(idx + sp)) {
      element_t t = get(idx + sp);
      set(idx + sp, get(idx));
      set(idx, t);
    }

    if(idx + sp == N_COLUMNS - 1) {
      idx = 0;
      sp -= 1;
      if(sp <= 1) {
        SetFinished(true);
      }
    } else {
      ++idx;
    }
  }

  int8_t idx, sp;
};

struct CocktailSort : public SortBase {

  virtual void init() {
    SortBase::init();
    idx = 0;
    min_idx = 0;
    max_idx = N_COLUMNS - 1;
    dir = 1;
  }

  virtual void step() {
    bool const pos = dir > 0;
    if(dir * (get(idx) - get(idx + dir)) > 0) {
      element_t t = get(idx + dir);
      set(idx + dir, get(idx));
      set(idx, t);
    }
    if(idx + dir == (pos ? max_idx : min_idx)) {
      if(pos) {
        max_idx -= 1;
      } else {
        min_idx += 1;
      }
      dir *= -1;
      if(max_idx - min_idx <= 1) {
        SetFinished(true);
      }
    } else {
      idx += dir;
    }
  }

  int8_t idx, min_idx, max_idx, dir;
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

#endif
