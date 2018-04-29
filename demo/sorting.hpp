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
    addSorter(new CombSort());
    addSorter(new InsertionSort());
    addSorter(new SelectionSort());
    addSorter(new ShellSort());
    __current->init();
  }

  virtual void updateImpl() {
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

  virtual Value getValue(Index col, Index row) const {
    return row == __current->getElement(col)
      ? __current->getWriteActivity(col) * ON_HIGH + (1.0f - __current->getWriteActivity(col)) * ON
      : __current->getReadActivity(col) * OFF_HIGH + (1.0f - __current->getReadActivity(col)) * OFF;
  }

  virtual float getFrameMSImpl() const {
    if(__paused) {
      return (WRITE_TIME + READ_TIME) * 10;
    } else {
      return WRITE_TIME * __current->getWriteCount() + READ_TIME * __current->getReadCount();
    }
  }

private:
  void addSorter(SortBase * _val) {
    if(__current == 0) {
      __current = _val;
      _val->setNext(_val);
    } else {
      _val->setNext(__current);
      __current->getLast()->setNext(_val);
    }
  }

  SortBase * __current;
  bool __paused;
};
