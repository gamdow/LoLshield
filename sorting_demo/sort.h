#ifndef sort_h
#define sort_h

#include "lolshield.h"

typedef uint8_t element_t;

struct SortBase {
  SortBase() {
    initElements();
    finished = true;
  }

  void initElements() {
    for(byte i = 0; i < N_COLUMNS; ++i) {
      elements[i] = random(0, N_ROWS);
      reads[i] = 0.f;
      writes[i] = 0.f;
    }
    read_count = 0;
    write_count = 0;
  }

  void decayHightlights(float decay) {
    for(byte i = 0; i < N_COLUMNS; ++i) {
      reads[i] *= decay;
      writes[i] *= decay;
    }
    read_count = 0;
    write_count = 0;
  }

  inline void step() {stepImpl();}
  inline void setFinished(bool value) {finished = value;}
  inline bool getFinished() const {return finished;}

  void init() {
    initElements();
    finished = false;
    initImpl();
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

  static element_t elements[N_COLUMNS];
  static float reads[N_COLUMNS];
  static float writes[N_COLUMNS];
  static int read_count;
  static int write_count;

private:
  virtual void initImpl() = 0;
  virtual void stepImpl() = 0;
  bool finished;
};

element_t SortBase::elements[N_COLUMNS];
float SortBase::reads[N_COLUMNS];
float SortBase::writes[N_COLUMNS];
int SortBase::read_count = 0;
int SortBase::write_count = 0;

struct BubbleSortBase : public SortBase {
  virtual void initImpl() {
    min_idx = 0;
    max_idx = N_COLUMNS - 1;
    idx = max_idx;
    sp = 1;
    dir = -1;
    initBubbleImpl();
  }

  virtual void initBubbleImpl() {}

  void compSwap(int _l, int _r) {
    if(dir * (get(_l) - get(_r)) > 0) {
      element_t t = get(_r);
      set(_r, get(_l));
      set(_l, t);
    }
  }

  virtual void onBoundary() = 0;

  virtual void stepImpl() {
    int n_idx = idx + dir * sp;
    compSwap(idx, n_idx);
    if(n_idx == (dir > 0 ? max_idx : min_idx)) {
      onBoundary();
      if(max_idx - min_idx < 1 || sp < 1) {
        setFinished(true);
      }
    } else {
      idx += dir;
    }
  }

  int8_t min_idx, max_idx, idx, sp, dir;
};

struct BubbleSort : public BubbleSortBase {
  virtual void onBoundary() {
    if(dir > 0) {
      max_idx -= 1;
    } else {
      min_idx += 1;
    }
    idx = dir > 0 ? min_idx : max_idx;
  }
};

struct CocktailSort : public BubbleSortBase {
  virtual void onBoundary() {
    if(dir > 0) {
      max_idx -= 1;
    } else {
      min_idx += 1;
    }
    dir *= -1;
  }
};

struct CombSort : public BubbleSortBase {
  virtual void initBubbleImpl() {sp = N_COLUMNS - 1;}
  virtual void onBoundary() {
    idx = dir > 0 ? min_idx : max_idx;
    sp -= 1;
  }
};

struct SelectionSort : public SortBase {
  virtual void initImpl() {
    start_idx = 0;
    idx = 1;
    min_idx = 0;
  }

  virtual void stepImpl() {
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
      if(idx == N_COLUMNS) {
        setFinished(true);
      }
    }
  }

  int8_t idx, start_idx, min_idx;
};

struct InsertionSort : public SortBase {
  virtual void initImpl() {
    idx = 0;
    end_idx = 1;
    sortValue = get(end_idx);
  }

  virtual void stepImpl() {
    if(idx == -1 || get(idx) <= sortValue)
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

  int8_t idx, end_idx;
  element_t sortValue;
};

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
    if(idx < start_idx || get(idx) <= sortValue)
    {
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

  int8_t idx, start_idx, end_idx, space_idx, sp;
  element_t sortValue;
};

#endif
