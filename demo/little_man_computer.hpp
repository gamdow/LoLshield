#pragma once

#include "demo_base.hpp"
#include "little_man_computer/params.hpp"
#include "little_man_computer/programs.hpp"

void lerp(int & _from, int _to) {
  _from += _to < _from ? -1 : 1;
}

struct Program {
  Program(LMC::InstructionType const * _prog)
    : __prog(_prog)
  {
    reset();
  }

  void reset() {
    __counter = 0;
    __target = 0;
    __override.active = false;
  }

  int getCounter() const {return __counter;}
  LMC::InstructionType getInstruction() const {return __prog[__counter];}
  LMC::ValueType getValue() const {
    if(__override.active) {
      return __override.value;
    } else if(LMC::hasValue(getInstruction())) {
      return __prog[__counter + 1];
    } else {
      return 0;
    }
  }

  void setValue(LMC::ValueType _val) {
    __override.active = true;
    __override.value = _val;
  }

  bool lerpComplete() const {return __counter == __target;}
  void lerpCounter() {
    lerp(__counter, __target);
    __override.active = false;
  }

  void jump() {__target = getValue();}
  void advance() {
    __counter += LMC::hasValue(getInstruction()) ? 2 : 1;
    __target = __counter;
    __override.active = false;
  }

private:
  LMC::InstructionType const * const __prog;
  int __counter;
  int __target;
  struct {
    bool active;
    LMC::ValueType value;
  } __override;
};

struct LittleManComputer : public DemoBase {
  static int const MAX_REGISTER_VALUE = (2 << N_ROWS) - 1;
  static Value const OFF = 0u;
  static Value const ON = MAX_VALUE / 3;
  static Value const OFF_HIGH = 1u;
  static Value const ON_HIGH = MAX_VALUE;

  static float const MS_INSTRUCTION = 200.0f;
  static float const MS_COUNTER_LERP = 20.0f;
  static float const MS_MAN_LERP = 50.0f;
  static float const MS_INBOX_OUTBOX = 500.0f;

  enum Indexes {
    INBOX_INDEX = -1,
    FIRST_REG_INDEX,
    LAST_REG_INDEX = Display::N_COLUMNS - 3,
    PROG_COUNTER_INDEX = LAST_REG_INDEX,
    INSTRUCTION_INDEX,
    VALUE_INDEX,
    OUTBOX_INDEX = Display::N_COLUMNS,

    NUM_REGISTERS = LAST_REG_INDEX - FIRST_REG_INDEX
  };

  LittleManComputer()
    : __program(LMC::primeFactory)
    , __man(0)
    , __stack_index(N_COLUMNS - 1)
  {
    for(Index i = 0; i < NUM_REGISTERS; i++) {
      __reg[i] = 0;
    }
    for(Index i = 0; i < N_COLUMNS; i++) {
      __in[i] = random(0, 20);
      __out[i] = 0;
    }
  }

private:
  bool popInboxSubStep() {
    if(__stack_index == N_COLUMNS - 1) {
      __acc = __in[__stack_index];
      __frame_ms = MS_INBOX_OUTBOX;
    } else if(__stack_index >= 0) {
      __in[__stack_index + 1] = __in[__stack_index];
      __frame_ms = MS_COUNTER_LERP;
    } else {
      __stack_index = N_COLUMNS - 1;
      __in[0] = random(2, 20);
      __frame_ms = MS_COUNTER_LERP;
      return true;
    }
    __in[__stack_index] = 0;
    --__stack_index;
    return false;
  }

  bool pushOutboxSubStep() {
    if(__stack_index > 0) {
      --__stack_index;
      __out[__stack_index + 1] = __out[__stack_index];
      __out[__stack_index] = 0;
      __frame_ms = MS_COUNTER_LERP;
      return false;
    } else {
      __stack_index = N_COLUMNS - 1;
      __out[0] = __acc;
      __frame_ms = MS_INBOX_OUTBOX;
      return true;
    }
  }

  void processNextInstruction() {
    using namespace LMC;
    __frame_ms = MS_INSTRUCTION;
    int ins = __program.getInstruction();
    int val = __program.getValue();
    if(LMC::hasAddressMemory(ins) && LMC::isPointerValue(val)) {
      val = LMC::getPointerlessValue(val);
      __program.setValue(__reg[val]);
    } else {
      switch(__program.getInstruction()) {
        case INBOX: if(popInboxSubStep()) __program.advance(); break;
        case OUTBOX: if(pushOutboxSubStep()) __program.advance(); break;
        case COPY_FROM: __acc = __reg[val]; __program.advance(); break;
        case COPY_TO: __reg[val] = __acc; __program.advance(); break;
        case ADD_FROM: __acc += __reg[val]; __program.advance(); break;
        case SUB_FROM: __acc -= __reg[val]; __program.advance(); break;
        case ADD_ONE_TO: __acc = ++__reg[val]; __program.advance(); break;
        case SUB_ONE_TO: __acc = --__reg[val]; __program.advance(); break;
        case JUMP: __program.jump(); break;
        case JUMP_IF_ZERO: if(__acc == 0) __program.jump(); else __program.advance(); break;
        case JUMP_IF_NEG: if(__acc < 0) __program.jump(); else __program.advance(); break;
        case JUMP_IF_POS: if(__acc > 0) __program.jump(); else __program.advance(); break;
        default:
          __program.reset();
          __frame_ms = 10 * MS_INSTRUCTION;
          break;
      }
    }
  }

  bool lerpMan(int _to) {
    if(__man != _to) {
      lerp(__man, _to);
      __frame_ms = MS_MAN_LERP;
      return true;
    } else {
      return false;
    }
  }

  bool lerpProgramAddress() {
    if(!__program.lerpComplete()) {
      if(!lerpMan(PROG_COUNTER_INDEX)) {
        __program.lerpCounter();
        __frame_ms = MS_COUNTER_LERP;
      }
      return true;
    } else {
      return false;
    }
  }

  int getManTarget() {
    using namespace LMC;
    int ins = __program.getInstruction();
    switch (ins) {
      case INBOX: return INBOX_INDEX;
      case OUTBOX: return OUTBOX_INDEX;
      default: {
        if(LMC::hasAddressMemory(ins)) {
          return LMC::getPointerlessValue(__program.getValue()) + FIRST_REG_INDEX;
        } else {
          return __man;
        }
      }
    }
  }

  virtual void __update() {
    if(lerpProgramAddress()) {
      return;
    } else if(lerpMan(getManTarget())) {
      return;
    } else {
      processNextInstruction();
    }
  }

  virtual Value __getValue(Index _col, Index _row) const {
    _col = N_COLUMNS - 1 - _col;
    int val;
    if(__man >= OUTBOX_INDEX) {
      val = __out[_col];
    } else if(__man <= INBOX_INDEX) {
      val = __in[_col];
    } else {
      switch(_col) {
        case PROG_COUNTER_INDEX: val = __program.getCounter(); break;
        case INSTRUCTION_INDEX: val = __program.getInstruction(); break;
        case VALUE_INDEX: val = __program.getValue(); break;
        default: val = __reg[_col - FIRST_REG_INDEX]; break;
      }
    }

    if(_col == (__man + N_COLUMNS) % N_COLUMNS) {
      return min(MAX_VALUE, (val & (1 << _row) ? OFF_HIGH : OFF) + (__acc & (1 << _row) ? ON_HIGH : OFF_HIGH));
    } else {
      return val & (1 << _row) ? ON : OFF;
    }
  }

  virtual float __getFrameMS() const {return __frame_ms;}

  Program __program;
  int __man;
  int __acc;
  int __stack_index;
  float __frame_ms;
  int __in[N_COLUMNS];
  int __reg[NUM_REGISTERS];
  int __out[N_COLUMNS];
};
