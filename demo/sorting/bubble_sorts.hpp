#pragma once

#include "sort_base.hpp"

struct BubbleSortBase : public SortBase {
protected:
  void shrinkBoundary() {
    if(__dir_sp > 0) {
      __max -= 1;
    } else {
      __min += 1;
    }
  }

  void resetIndex() {
    __idx = __dir_sp > 0 ? __min : __max;
  }

  SignedIndex __min, __max, __idx, __comp_sp, __dir_sp;

private:
  SignedElement getSigned(Index _i) const {return static_cast<SignedElement>(get(_i));}

  bool swapIfGreater(int _l, int _r) {
    if(__dir_sp * (getSigned(_l) - getSigned(_r)) > 0) {
      Element t = get(_r);
      set(_r, get(_l));
      set(_l, t);
      return true;
    } else {
      return false;
    }
  }

  virtual void __init() {
    __min = 0;
    __max = N_COLUMNS - 1;
    __idx = __min;
    __comp_sp = 1;
    __dir_sp = 1;
    __any_swaps = false;
    __initBubble();
  }

  virtual void __step() {
    do {
      SignedIndex compare_index = __idx + (__dir_sp > 0 ? 1 : -1) * __comp_sp;
      bool hit_boundary =  __dir_sp > 0 ? compare_index > __max : compare_index < __min;
      if(hit_boundary) {
        bool check_sorted = __onBoundary();
        if(check_sorted && !__any_swaps || __max - __min < 1 || __comp_sp < 1) {
          setFinished(true);
        } else if(check_sorted) {
          __any_swaps = false;
        }
      } else {
        bool swapped = swapIfGreater(__idx, compare_index);
        __any_swaps = __any_swaps || swapped;
        __idx += __dir_sp;
      }
    } while(__anotherStep());
  }

  virtual void __initBubble() {}
  virtual bool __onBoundary() = 0;
  virtual bool __anotherStep() {return false;}

  bool __any_swaps;
};

struct BubbleSort : public BubbleSortBase {
private:
  virtual bool __onBoundary() {
    shrinkBoundary();
    resetIndex();
    return true;
  }
};

struct CocktailSort : public BubbleSortBase {
private:
  virtual bool __onBoundary() {
    shrinkBoundary();
    __dir_sp *= -1;
    return true;
  }
};

struct CombSort : public BubbleSortBase {
private:
  virtual void __initBubble() {
    __comp_sp = N_COLUMNS - 1;
  }
  virtual bool __onBoundary() {
    __comp_sp -= 1;
    resetIndex();
    return false;
  }
};

struct OddEvenSort : public BubbleSortBase {
  OddEvenSort()
    : __boundary_hit(false)
  {}

private:
  void parity(int i) {
    i = i % 2;
    __min = i;
    __max = N_COLUMNS % 2 == i ? N_COLUMNS - 1 : N_COLUMNS - 2;
  }

  virtual void __initBubble() {
    __dir_sp = 2;
    parity(0);
  }

  virtual bool __onBoundary() {
    parity(__dir_sp > 0 ? 1 : 0);
    __dir_sp *= -1;
    resetIndex();
    __boundary_hit = true;
    return __min == 0;
  }

  virtual bool __anotherStep() {
    if(__boundary_hit) {
      __boundary_hit = false;
      return false;
    } else {
      return true;
    }
  }

  bool __boundary_hit;
};
