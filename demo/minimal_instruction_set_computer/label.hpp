#pragma once

#include "expression.hpp"

namespace MISC {

struct Label : public Expression {
private:
  static int __id;
public:
  Label() : Expression(0, __id++, 0, false) {}
  Expression operator()(Expression const & _v) const {return Expression(Expression(__label_ref, 0, 0, false), _v);}
  Expression operator()() const {return Expression(Expression(__label_ref, 0, 0, false), Expression(0));}
};

int Label::__id = 1;

struct LabelMap {
public:
  LabelMap(Expression const * _prog, size_t _n);
  ~LabelMap() {delete [] __id; delete [] __address;}
  e_type operator[](id_type id) const;
  void print() const;
private:
  id_type * __id;
  e_type * __address;
  size_t __size;
};

LabelMap::LabelMap(Expression const * _prog, size_t _n)
  : __id(0)
  , __address(0)
{
  __size = 0;
  for(size_t i = 0; i < _n; ++i) if(_prog[i].isLabel()) ++__size;
  __id = new id_type[__size]();
  __address = new e_type[__size]();
  size_t count = 0;
  for(size_t i = 0; i < _n; ++i) {
    Expression const & e = _prog[i];
    if(e.isLabel()) {
      __id[count] = e.label_id();
      __address[count] = i;
      ++count;
    }
  }
}

e_type LabelMap::operator[](id_type id) const {
  for(size_t i = 0; i < __size; ++i) {
    if(__id[i] == id) {
      return __address[i];
    }
  }
  return 0;
}

void LabelMap::print() const {
#ifdef DEBUG_MODE
  Serial.println("LabelMap:");
  for(size_t i = 0; i < __size; ++i) {
    Serial.print(__id[i]);
    Serial.print(' ');
    Serial.println(__address[i]);
  }
  Serial.println(' ');
#endif
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
