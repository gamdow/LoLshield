#pragma once

#include "instruction_set.hpp"
#include "program.hpp"

namespace MISC {

struct Animation {
  virtual float getState(int _row, int _height, int _frame) const = 0;
  virtual bool isAnimating(int _height, int _frame) const {return _frame < _height;}
};

struct AnimNone : public Animation {
  virtual float getState(int _row, int _height, int _frame) const {return 0.0f;}
  virtual bool isAnimating(int _height, int _frame) const {return false;}
};

struct AnimIncrement : public Animation {
  float get(int _row, int _frame) const {return (_row % 3) == (_frame % 3) ? 1.0f : 0.0f;}
  virtual float getState(int _row, int _height, int _frame) const {return get(_row, _frame) + get(_row + 1, _frame) * 0.2f;}
};

struct AnimDecrement : public AnimIncrement {
  virtual float getState(int _row, int _height, int _frame) const {
    return AnimIncrement::getState(_height - _row - 1, _height, _frame);
  }
};

struct AnimStore : public Animation {
  float get(int _row, int _frame, int _height) const {return _row < _frame && _row > _frame - _height ? 1.0f : 0.0f;}
  virtual float getState(int _row, int _height, int _frame) const {
    int half = _height / 2 + 1;
    return get(_row < half ? _row : _height - _row, _frame, half);
  }
};

struct AnimLoad : public AnimStore {
  virtual float getState(int _row, int _height, int _frame) const {
    return AnimStore::getState(_row, _height, _height - _frame - 1);
  }
};

struct AnimLerp : public Animation {
  virtual float getState(int _row, int _height, int _frame) const {
    int val = from + _frame * float(from - to) / _height;
    return val & (1 << _row);
  }
  int from, to;
};

template<size_t HEIGHT>
struct AnimationManager {
  AnimationManager()
    : __animation(0)
    , __frame(0)
  {
    init(-1, 0, 0);
  }

  void init(Program const & _program) {
    init(_program.getNextInstruction(), _program.getAccumulator(), _program.getNextValue());
  }

  bool animate() {
    if(__animation->isAnimating(HEIGHT, __frame)) {
      __frame += 1;
      return true;
    } else {
      return false;
    }
  }

  bool isAnimating() const {
    return __frame >= 0 && __animation->isAnimating(HEIGHT, __frame);
  }

  float operator[](int i) const {
    return __animation->getState(i, HEIGHT, __frame);
  }

private:
  void init(int _ins, int _bg, int _fg) {
    __frame = -1;
    switch(_ins) {
      case INC: __animation = &__increment; break;
      case DEC: __animation = &__decrement; break;
      case LOAD: __animation = &__load; break;
      case STORE: __animation = &__store; break;
      case ADD:
      case SUB: __lerp.from = _bg, __lerp.to = _fg; __animation = &__lerp; break;
      default: __animation = &__none; break;
    }
  }

  AnimNone __none;
  AnimIncrement __increment;
  AnimDecrement __decrement;
  AnimLoad __load;
  AnimStore __store;
  AnimLerp __lerp;
  Animation * __animation;
  int __frame;
};

}
