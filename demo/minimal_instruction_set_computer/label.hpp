#pragma once

//#include <cstddef>

#include "expression.hpp"

namespace MISC {

struct Label : public Expression {
private:
  static int __id;
public:
  Label() : Expression(0, __id++, 0, false) {}
  Expression operator()(Expression const & _v) const;
  Expression operator()() const;
};

int Label::__id = 1;

Expression Label::operator()(Expression const & _v) const {
  return Expression(Expression(__label_ref, 0, 0, false), _v);
}

Expression Label::operator()() const {
  return Expression(Expression(__label_ref, 0, 0, false), Expression(0));
}

struct LabelMap {
private:
  void construct(Expression const * _prog, size_t _n);
public:
  template<size_t N> LabelMap(Expression const (&_prog)[N]) : __count(0) {construct(_prog, N);}
  int operator[](int id) const;
private:
  static size_t const MAX = 20u;
  int __id[MAX];
  int __address[MAX];
  size_t __count;
};

int LabelMap::operator[](int id) const {
  for(size_t i = 0; i < __count; ++i) {
    if(__id[i] == id) {
      return __address[i];
    }
  }
  return 0;
}

void LabelMap::construct(Expression const * _prog, size_t _n) {
  Serial.print("LabelMap::construct ");
  Serial.println(_n);
  using namespace MISC;
  for(size_t i = 0; i < _n; ++i) {
    Expression const & e = _prog[i];
    if(e.isLabel()) {
      __id[__count] = e.label_id();
      __address[__count] = i;
      ++__count;
    }
  }
}

// Label const A;
// Label const B;
// Label const C;
// Label const D;
// Label const E;
// Label const F;
// Label const G;
// Label const H;
// Label const I;
// Label const J;
// Label const K;
// Label const L;
// Label const M;
// Label const N;
// Label const O;
// Label const P;
// Label const Q;
// Label const R;
// Label const S;
// Label const IN;
// Label const OUT;
// Label const START;
// Label const END;
// Label const LOOP;

}
