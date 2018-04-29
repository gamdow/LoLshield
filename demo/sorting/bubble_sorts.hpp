#pragma once

#include "sort_base.hpp"

struct BubbleSortBase : public SortBase {
protected:
  SignedIndex min_idx, max_idx, idx, sp, dir;

private:
  SignedElement getSignedElement(Index i) const {return static_cast<SignedElement>(get(i));}

  virtual void initImpl() {
    min_idx = 0;
    max_idx = DemoParams::N_COLUMNS - 1;
    idx = max_idx;
    sp = 1;
    dir = -1;
    initBubbleImpl();
  }

  virtual void initBubbleImpl() {}

  void compSwap(int _l, int _r) {
    if(dir * (getSignedElement(_l) - getSignedElement(_r)) < 0) {
      Element t = get(_r);
      set(_r, get(_l));
      set(_l, t);
    }
  }

  virtual void onBoundary() = 0;

  virtual void stepImpl() {
    SignedIndex n_idx = idx + dir * sp;
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
};

struct BubbleSort : public BubbleSortBase {
private:
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
private:
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
private:
  virtual void initBubbleImpl() {sp = DemoParams::N_COLUMNS - 1;}
  virtual void onBoundary() {
    idx = dir > 0 ? min_idx : max_idx;
    sp -= 1;
  }
};
