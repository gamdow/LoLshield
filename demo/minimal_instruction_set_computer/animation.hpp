#pragma once

#include "instruction_set.hpp"

namespace MISC {

struct Animation {
  virtual float getState(int _row, int _height, int _frame) {return 0.0f;}
  virtual bool isAnimating(int _height, int _frame) const {return false;}
};

struct AnimIncrement : public Animation {
  virtual float getState(int _row, int _height, int _frame) {
    return (_row % 3) == (_frame % 3) ? 1.0f
      : ((_row + 1) % 3) == (_frame % 3) ? 0.2f : 0.0f;
  }
  virtual bool isAnimating(int _height, int _frame) const {return _frame < _height;}
};

struct AnimDecrement : public AnimIncrement {
  virtual float getState(int _row, int _height, int _frame) {
    return AnimIncrement::getState(_height - _row - 1, _height, _frame);
  }
};

struct AnimStore : public Animation {
  virtual float getState(int _row, int _height, int _frame) {
    return _row <= _height / 2
      ? (_row < _frame && _row > _frame - 5 ? 1.0f : 0.0f)
      : (_height - _row < _frame && _height - _row > _frame - 5 ? 1.0f : 0.0f);
  }
  virtual bool isAnimating(int _height, int _frame) const {return _frame < _height;}
};

struct AnimLoad : public AnimStore {
  virtual float getState(int _row, int _height, int _frame) {
    return AnimStore::getState(_row, _height, _height - _frame - 1);
  }
};

struct AnimLerp : public Animation {
  virtual float getState(int _row, int _height, int _frame) {
    float speed = float(abs(from - to)) / _height;
    int val = from + (to > from ? _frame : -_frame) * speed;
    return val & (1 << _row);
  }
  virtual bool isAnimating(int _height, int _frame) const {return _frame <  _height;}
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
  Animation __none;
  AnimIncrement __increment;
  AnimDecrement __decrement;
  AnimLoad __load;
  AnimStore __store;
  AnimLerp __lerp;
  Animation * __animation;
  int __frame;
};

}
