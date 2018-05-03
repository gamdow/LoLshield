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

  Sorting()
    : __current(0)
    , __paused(true)
  {
    addSorter(new BubbleSort());
    addSorter(new CocktailSort());
    addSorter(new OddEvenSort());
    addSorter(new CombSort());
    addSorter(new InsertionSort());
    addSorter(new ShellSort());
    addSorter(new SelectionSort());
    __current->init();
  }

private:
  virtual void __update() {
    if(__current->getFinished()) {
      if(__paused) {
        __current = __current->getNext();
        __current->init();
      } else {
        __current->decayActivity(0.0f);
        __paused = true;
      }
    } else {
      if(__paused) {
        __current->decayActivity(0.0f);
        __paused = false;
      } else {
        __current->decayActivity(0.5f);
        __current->step();
      }
    }
  }

  virtual Value __getValue(Index _col, Index _row) const {
    _col = N_COLUMNS - 1 - _col;
    return _row == __current->getElement(_col)
      ? __current->getWriteActivity(_col) * ON_HIGH + (1.0f - __current->getWriteActivity(_col)) * ON
      : __current->getReadActivity(_col) * OFF_HIGH + (1.0f - __current->getReadActivity(_col)) * OFF;
  }

  virtual float __getFrameMS() const {
    if(__paused) {
      return (WRITE_TIME + READ_TIME) * 10;
    } else {
      return WRITE_TIME * __current->getWriteCount() + READ_TIME * __current->getReadCount();
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

  SortBase * __current;
  bool __paused;
};
