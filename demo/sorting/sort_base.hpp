#pragma once

#include "../display_params.hpp"
#include "../link_list.hpp"

struct SortBase : public Node {
  typedef typename Display::IndexType Index;
  typedef typename Display::ValueType Element;
  typedef int SignedIndex;
  typedef int SignedElement;
  static Index const N_COLUMNS = Display::N_COLUMNS;
  static Index const N_ROWS = Display::N_ROWS;

  void init() {
    initElements();
    __finished = false;
    __init();
  }

  void initElements() {
    for(Index i = 0; i < N_COLUMNS; ++i) {
      __elements[i] = random(0, N_ROWS);
    }
    decayActivity(0.f);
  }

  void decayActivity(float decay) {
    for(Index i = 0; i < N_COLUMNS; ++i) {
      __reads[i] *= decay;
      __writes[i] *= decay;
    }
    __read_count = 0;
    __write_count = 0;
  }

  inline Element getElement(Index i) const {return __elements[i];}
  inline float getReadActivity(Index i) const {return __reads[i];}
  inline float getWriteActivity(Index i) const {return __writes[i];}
  inline int getReadCount() const {return __read_count;}
  inline int getWriteCount() const {return __write_count;}
  inline void step() {__step();}
  inline void setFinished(bool value) {__finished = value;}
  inline bool getFinished() const {return __finished;}

protected:
  Element get(Index i) const {
    ++__read_count;
    __reads[i] = 1.0f;
    return __elements[i];
  }

  void set(Index i, Element e) {
    ++__write_count;
    __writes[i] = 1.0f;
    __elements[i] = e;
  }

private:
  virtual void __init() = 0;
  virtual void __step() = 0;
  static Element __elements[N_COLUMNS];
  static float __reads[N_COLUMNS];
  static float __writes[N_COLUMNS];
  static int __read_count;
  static int __write_count;
  bool __finished;
};

SortBase::Element SortBase::__elements[SortBase::N_COLUMNS];
float SortBase::__reads[SortBase::N_COLUMNS];
float SortBase::__writes[SortBase::N_COLUMNS];
int SortBase::__read_count = 0;
int SortBase::__write_count = 0;
