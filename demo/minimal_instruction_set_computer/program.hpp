#pragma once

#include "program_listings.hpp"

namespace MISC {

struct Program {
  typedef int16_t e_type;
  template<typename T>
  static T bitShift(T _v, size_t o, size_t n) {return (~(-1 << n) & _v) << o;}
  static e_type bitMask(size_t o, size_t n) {return bitShift<e_type>(-1, o, n);}
  static size_t msbIndex(int _v);
  template<size_t N> Program(MISC::Expression const (&_prog)[N]) : __prog(_prog), __prog_size(N), __mem(0) {init();}
  ~Program() {delete [] __mem;}
  int operator[](int i) const {return __mem[i];}
  e_type getNextInstruction() const {return getInstruction(__mem[__counter]);}
  int getNextAddress() const {return isNodref(__mem[__counter]) ? __counter + 1 : __mem[__counter + 1];}
  e_type & getNextValue() {return __mem[getNextAddress()];}
  e_type const & getNextValue() const {return __mem[getNextAddress()];}
  int getCounter() const {return __counter;}
  e_type getAccumulator() const {return __acc;}
  void reset();
  bool next();
  void print();
private:
  static size_t const DATA_SIZE = 100u;
  static size_t const VALUE_BITS = 8u;
  static size_t const SIGN_OFFSET = 7u;
  static e_type const VALUE_MASK;
  static e_type const SIGN_MASK;
  static e_type const INSTRUCTION_MASK;
  static int getValue(e_type _e) {return (_e & VALUE_MASK) | ((_e & SIGN_MASK) ? ~VALUE_MASK : 0);}
  static int getInstruction(e_type _e) {return _e & INSTRUCTION_MASK;}
  void printState(e_type val);
  void init();
  Expression const * const __prog;
  size_t const __prog_size;
  e_type * __mem;
  e_type __acc;
  int __counter;
};

void Program::init() {
  __acc = 0;
  __counter = 0;
  delete [] __mem;
  LabelMap labels(__prog, __prog_size);
  labels.print();
  __mem = new e_type[__prog_size + DATA_SIZE]();
  for(size_t i = 0; i < __prog_size; ++i) {
    Expression const & e = __prog[i];
    __mem[i] = e.value()
      + (e.isRef() ? labels[e.label_ref()] : 0);
  }
  print();
}

void Program::reset() {
  init();
}

void Program::print() {
  Serial.println("Program:");
  for(size_t i = 0; i < __prog_size; ++i) {
    Serial.print(i);
    Serial.print(':');
    Serial.println(__mem[i]);
  }
}

void Program::printState(e_type val) {
  Serial.print(__counter);
  Serial.print(' ');
  Serial.print(val);
  Serial.print(' ');
  Serial.print(__acc);
}

bool Program::next() {
  e_type ins = getNextInstruction();
  e_type & val = getNextValue();
#ifdef DEBUG_MODE
  printState(val);
  Serial.print(" >");
  Serial.print(ins);
  Serial.print("> ");
#endif
  __counter += 2;
  switch(ins) {
    case LOAD: __acc = val;  break;
    case STORE: val = __acc; break;
    case ADD: __acc += val; break;
    case SUB: __acc -= val; break;
    case INC: __acc = ++val; break;
    case DEC: __acc = --val; break;
    // case COUNT: val = isData(val) ? Expression(DATA, __counter) : Expression(__counter); break;
    case JMP: __counter = val; break;
    case JMP_Z: if(__acc == 0) __counter = val; break;
    case JMP_N: if(__acc < 0) __counter = val; break;
    case JMP_P: if(__acc > 0) __counter = val; break;
    case HALT:
      __counter -= 1;
#ifdef DEBUG_MODE
      Serial.println(" HALT");
#endif
      return false;
    default: __counter -= 1; break;
  }
#ifdef DEBUG_MODE
  printState(val);
  Serial.println(' ');
#endif
  return __counter >= 0 && __counter < __prog_size + DATA_SIZE - 1;
}

Program::e_type const Program::VALUE_MASK = Program::bitMask(0, VALUE_BITS);
Program::e_type const Program::SIGN_MASK = Program::bitMask(SIGN_OFFSET, 1);
Program::e_type const Program::INSTRUCTION_MASK = MISC::NODREF_MOD - 1;

}
