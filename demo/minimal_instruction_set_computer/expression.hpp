#pragma once

namespace MISC {

typedef int16_t e_type;
typedef int8_t id_type;

struct Expression {
  Expression(int _val);
  Expression(int _id, int _ref, int _val, bool _dref);
  Expression(Expression const & _l, Expression const & _r);
  void print() {
#ifdef DEBUG_MODE
    Serial.print("__label_id:");
    Serial.print(__label_id);
    Serial.print("__label_ref:");
    Serial.print(__label_ref);
    Serial.print("__value:");
    Serial.print(__value);
    Serial.print("__no_deref:");
    Serial.print(__no_deref);
#endif
  }
  bool isLabel() const {return __label_id > 0;}
  bool isRef() const {return __label_ref > 0;}
  id_type label_id() const {return __label_id;}
  id_type label_ref() const {return __label_ref;}
  e_type value() const {return __value;}
  bool no_deref() const {return __no_deref;}
protected:
  id_type __label_id;
  id_type __label_ref;
  e_type __value;
  bool __no_deref;
};

Expression operator+(Expression const & _l, int _r);
Expression operator-(Expression const & _l, int _r);
Expression operator+(Expression const & _l, Expression const &  _r);

Expression::Expression(int _val)
  : __label_id(0)
  , __label_ref(0)
  , __value(_val)
  , __no_deref(false)
{
}

Expression::Expression(int _id, int _ref, int _val, bool _dref)
  : __label_id(_id)
  , __label_ref(_ref)
  , __value(_val)
  , __no_deref(_dref)
{
}

Expression::Expression(Expression const & _l, Expression const & _r)
  : __label_id(_l.__label_id | _r.__label_id)
  , __label_ref(_l.__label_ref | _r.__label_ref)
  , __value(_l.__value | _r.__value)
  , __no_deref(_l.__no_deref | _r.__no_deref)
{
}

Expression operator+(Expression const & _l, int _r) {
  return Expression(_l, Expression(0, 0, _r, false));
}

Expression operator-(Expression const & _l, int _r) {
  return Expression(_l, Expression(0, 0, -_r, false));
}

Expression operator+(Expression const & _l, Expression const &  _r) {
  return Expression(_l, _r);
}

}
