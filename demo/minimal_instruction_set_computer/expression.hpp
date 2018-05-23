#pragma once

namespace MISC {

struct Expression {
  Expression(int _val);
  Expression(int _id, int _ref, int _val, bool _dref);
  Expression(Expression const & _l, Expression const & _r);
  bool isLabel() const {return __label_id > 0;}
  bool isRef() const {return __label_ref > 0;}
  int label_id() const {return __label_id;}
  int label_ref() const {return __label_ref;}
  int value() const {return __value;}
  bool no_deref() const {return __no_deref;}
  // void print() const;
protected:
  int __label_id;
  int __label_ref;
  int __value;
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
  // assert((_l.__label_id & _r.__label_id) == 0);
  // assert((_l.__label_ref & _r.__label_ref) == 0);
  // assert((_l.__value & _r.__value) == 0);
  // assert(!(_l.__no_deref && _r.__no_deref));
}

// void Expression::print() const {
//   std::cout << __label_id << " "
//     << __label_ref << " "
//     << __value << " "
//     << __no_deref;
// }

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
