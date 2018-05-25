#pragma once

#include "../display_params.hpp"
#include "../link_list.hpp"

struct SortData {
  typedef typename Display::IndexType Index;
  typedef typename Display::ValueType Element;
  static Index const N_COLUMNS = Display::N_COLUMNS;
  static Index const N_ROWS = Display::N_ROWS;

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

  inline Element getElement(Index i) const {return __elements[i];}
  inline float getReadActivity(Index i) const {return __reads[i];}
  inline float getWriteActivity(Index i) const {return __writes[i];}
  inline int getReadCount() const {return __read_count;}
  inline int getWriteCount() const {return __write_count;}

  Element __elements[N_COLUMNS];
  mutable float __reads[N_COLUMNS];
  float __writes[N_COLUMNS];
  mutable int __read_count;
  int __write_count;
};

struct SortBase : public Node {
  typedef typename Display::IndexType Index;
  typedef typename Display::ValueType Element;
  typedef int SignedIndex;
  typedef int SignedElement;
  static Index const N_COLUMNS = Display::N_COLUMNS;
  static Index const N_ROWS = Display::N_ROWS;

  SortBase(SortData & _data) : __data(_data) {}
  virtual ~SortBase() {}

  void init() {
    __data.initElements();
    __finished = false;
    __init();
  }

  inline void step() {__step();}
  inline void setFinished(bool value) {__finished = value;}
  inline bool getFinished() const {return __finished;}

protected:
  Element get(Index i) const {return __data.get(i);}
  void set(Index i, Element e) {__data.set(i, e);}

private:
  virtual void __init() = 0;
  virtual void __step() = 0;
  SortData & __data;
  bool __finished;
};
