#pragma once

#include "program_listings.hpp"

namespace MISC {

struct Program {
  typedef unsigned int e_type;
  static e_type bitShift(int _v, size_t o, size_t n);
  static e_type bitMask(size_t o, size_t n);
  static size_t msbIndex(int _v);
  template<size_t N> Program(MISC::Expression const (&_prog)[N])
  {
    construct(MISC::LabelMap(_prog), _prog, N);
  }
  void print();
  void reset();
  bool next();
  int operator[](int i) const {return __mem[i];}
  int getNextInstruction() const;
  int getNextAddress() const;
  int & getNextValue();
  int const & getNextValue() const;
  int getCounter() const {return __counter;}
  int getAccumulator() const {return __acc;}
private:
  static size_t const VALUE_BITS = 8u;
  static size_t const SIGN_OFFSET = 7u;
  static e_type const VALUE_MASK;
  static e_type const SIGN_MASK;
  static e_type const INSTRUCTION_MASK;
  static int getValue(e_type _e) {return (_e & VALUE_MASK) | ((_e & SIGN_MASK) ? ~VALUE_MASK : 0);}
  static int getInstruction(e_type _e) {return _e & INSTRUCTION_MASK;}
  void construct(MISC::LabelMap const & _map, MISC::Expression const * _prog, size_t _n);
  static size_t const MAX_SIZE = 200u;
  int __mem[MAX_SIZE];
  int __counter;
  int __acc;
};

Program::e_type Program::bitShift(int _v, size_t o, size_t n) {
  return (~(-1 << n) & _v) << o;
}

Program::e_type Program::bitMask(size_t o, size_t n) {
  return bitShift(-1, o, n);
}

void Program::construct(MISC::LabelMap const & _map, MISC::Expression const * _prog, size_t _n) {
  Serial.println("Program::construct");
  size_t i = 0;
  for(; i < _n && i < MAX_SIZE; ++i) {
    Expression const & e = _prog[i];
    int a = (bitShift(e.value(), 0, VALUE_BITS)
      + (e.isRef() ? bitShift(_map[e.label_ref()], 0, VALUE_BITS) : 0));
    __mem[i] = a;
  }
  if(i == MAX_SIZE) {
    Serial.println("Program too long!");
  }
  for(; i < MAX_SIZE; ++i) {
    __mem[i] = 0;
  }
  reset();
}

int Program::getNextInstruction() const {
  return getInstruction(__mem[__counter]);
}

int Program::getNextAddress() const {
  int ins = __mem[__counter];
  return isNodref(ins) ? __counter + 1 : __mem[__counter + 1];
}

int & Program::getNextValue() {
  return __mem[getNextAddress()];
}

int const & Program::getNextValue() const {
  return __mem[getNextAddress()];
}

void Program::print() {
  // std::cout << "\n";
  // size_t i = 0;
  // while(i < SIZE || __mem[i] != 0) {
  //   int val = __mem[i];
  //   char const * instruction = getInstruction(val) < MISC::NUM_INSTRUCTIONS ? MISC::INSTRUCTION_NAMES[getInstruction(val)] : "";
  //   bool is_instruction = !(i % 2 || i >= SIZE);
  //   if(is_instruction) {
  //     std::cout << i << ": ";
  //   }
  //   std::cout << getValue(val) << " (" << (MISC::isNodrefMod(val) ? "ND" : "") << instruction << ") ";
  //   if(!is_instruction) {
  //     std::cout << "\n";
  //   }
  //   ++i;
  // }
  // std::cout << "\n\n";
}

void Program::reset() {
  __counter = 0;
}

bool Program::next() {
  int ins = getNextInstruction();
  int & val = getNextValue();
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
    default: __counter -= 1; break;
  }
  return ins != HALT;
}

Program::e_type const Program::VALUE_MASK = Program::bitMask(0, VALUE_BITS);
Program::e_type const Program::SIGN_MASK = Program::bitMask(SIGN_OFFSET, 1);
Program::e_type const Program::INSTRUCTION_MASK = MISC::NODREF_MOD - 1;

}
