#pragma once

#include "demo_base.hpp"
#include "sorting/sort_base.hpp"
#include "sorting/bubble_sorts.hpp"
#include "sorting/insertion_sort.hpp"
#include "sorting/selection_sort.hpp"
#include "sorting/shell_sort.hpp"

struct Sorting : public DemoBase {
  static Value const OFF = 0u;
  static Value const ON = MAX_VALUE / 3;
  static Value const OFF_HIGH = 1u;
  static Value const ON_HIGH = MAX_VALUE;
  static long const READ_TIME = 10L;
  static long const WRITE_TIME = 20L;
  static int const REPETITIONS = 8;

  Sorting()
    : __current(0)
    , __rep(0)
    , __paused(true)
  {
    addSorter(new BubbleSort(__data));
    addSorter(new CocktailSort(__data));
    addSorter(new OddEvenSort(__data));
    addSorter(new CombSort(__data));
    addSorter(new InsertionSort(__data));
    addSorter(new ShellSort(__data));
    addSorter(new SelectionSort(__data));
    __current->init();
  }

private:
  virtual void __update() {
    if(__current->getFinished()) {
      if(__paused) {
        if(__rep >= REPETITIONS) {
          __current = static_cast<SortBase*>(__current->getNext());
          __rep = 0;
        }
        __current->init();
        ++__rep;
      } else {
        __data.decayActivity(0.0f);
        __paused = true;
      }
    } else {
      if(__paused) {
        __data.decayActivity(0.0f);
        __paused = false;
      } else {
        __data.decayActivity(0.5f);
        __current->step();
      }
    }
  }

  virtual Value __getValue(Index _col, Index _row) const {
    _col = N_COLUMNS - 1 - _col;
    return _row == __data.getElement(_col)
      ? __data.getWriteActivity(_col) * ON_HIGH + (1.0f - __data.getWriteActivity(_col)) * ON
      : __data.getReadActivity(_col) * OFF_HIGH + (1.0f - __data.getReadActivity(_col)) * OFF;
  }

  virtual float __getFrameMS() const {
    if(__paused) {
      return (WRITE_TIME + READ_TIME) * 10;
    } else {
      return WRITE_TIME * __data.getWriteCount() + READ_TIME * __data.getReadCount();
    }
  }

  void addSorter(SortBase * _val) {
    if(__current == 0) {
      __current = _val;
    } else {
      __current->getLast()->setNext(_val);
    }
    _val->setNext(__current);
  }

  SortData __data;
  SortBase * __current;
  int __rep;
  bool __paused;
};
