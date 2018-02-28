#ifndef sort_h
#define sort_h

#include "lolshield.h"

typedef uint8_t element_t;

element_t elements[N_COLUMNS];
float highlights[N_COLUMNS];

void init_elements() {
  for(byte i = 0; i < N_COLUMNS; ++i ) {
    elements[i] = random(0, N_ROWS + 1);
  }
}

void init_hightlights() {
  for(byte i = 0; i < N_COLUMNS; ++i ) {
    highlights[i] = 0;
  }
}

void decay_hightlights(float decay) {
  for(byte i = 0; i < N_COLUMNS; ++i ) {
    highlights[i] *= decay;
  }
}

void set_hightlight(byte i) {
  highlights[i] = 1.0f;
}

struct SortBase {
  void SetFinished(bool value) {
    finished = value;
  }

  bool GetFinished() const {
    return finished;
  }

  SortBase() {
    init_elements();
    init_hightlights();
    finished = true;
  }

  virtual void init() = 0;
  virtual void step() = 0;

  bool finished;
};

struct BubbleSort : public SortBase {

  virtual void init() {
    init_elements();
    init_hightlights();
    idx = 0;
    max_idx = N_COLUMNS - 1;
    SetFinished(false);
  }

  virtual void step() {

    set_hightlight(idx);
    if(elements[idx] > elements[idx + 1]) {
      element_t t = elements[idx + 1];
      elements[idx + 1] = elements[idx];
      elements[idx] = t;
      set_hightlight(idx + 1);
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

  byte idx, max_idx;
};

struct CocktailSort : public SortBase {

  virtual void init() {
    init_elements();
    init_hightlights();
    idx = 0;
    min_idx = 0;
    max_idx = N_COLUMNS - 1;
    dir = 1;
    SetFinished(false);
  }

  virtual void step() {

    set_hightlight(idx);
    if(dir > 0) {
      if(elements[idx] > elements[idx + 1]) {
        element_t t = elements[idx + 1];
        elements[idx + 1] = elements[idx];
        elements[idx] = t;
        set_hightlight(idx + 1);
      }
      if(idx + 1 == max_idx) {
        dir = 0;
        max_idx -= 1;
        if(max_idx - min_idx <= 1) {
          SetFinished(true);
        }
      } else {
        ++idx;
      }
    } else {
      if(elements[idx] < elements[idx - 1]) {
        element_t t = elements[idx - 1];
        elements[idx - 1] = elements[idx];
        elements[idx] = t;
        set_hightlight(idx - 1);
      }
      if(idx - 1 == min_idx) {
        dir = 1;
        min_idx += 1;
        if(max_idx - min_idx <= 1) {
          SetFinished(true);
        }
      } else {
        --idx;
      }
    }
  }

  byte idx, min_idx, max_idx, dir;
};

struct SelectionSort : public SortBase {

  virtual void init() {
    init_elements();
    init_hightlights();
    start_idx = 0;
    idx = 1;
    min_idx = 0;
    SetFinished(false);
  }

  virtual void step() {

    if(idx < N_COLUMNS)
    {
      set_hightlight(min_idx);
      set_hightlight(idx);
      if(elements[idx] < elements[min_idx]) {
        min_idx = idx;
      }
      ++idx;
    } else {
      set_hightlight(start_idx);
      if(min_idx != start_idx) {
        set_hightlight(min_idx);
        element_t t = elements[min_idx];
        elements[min_idx] = elements[start_idx];
        elements[start_idx] = t;
      }
      ++start_idx;
      min_idx = start_idx;
      idx = start_idx + 1;
      if(idx + 1 == N_COLUMNS) {
        SetFinished(true);
      }
    }
  }

  byte idx, start_idx, min_idx;
};

struct InsertionSort : public SortBase {

  virtual void init() {
    init_elements();
    init_hightlights();
    idx = 0;
    end_idx = 1;
    sortValue = elements[end_idx];
    SetFinished(false);
  }

  virtual void step() {
    set_hightlight(idx + 1);
    if(idx == -1 or elements[idx] < sortValue)
    {
      elements[idx + 1] = sortValue;
      ++end_idx;
      if(end_idx == N_COLUMNS) {
        SetFinished(true);
      } else {
        sortValue = elements[end_idx];
        idx = end_idx - 1;
      }
    } else {
      elements[idx + 1] = elements[idx];
      --idx;
    }
  }

  int8_t idx, end_idx;
  element_t sortValue;
};

#endif
