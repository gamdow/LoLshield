#pragma once

#include "demo_base.hpp"
#include "minimal_instruction_set_computer/program.hpp"
#include "minimal_instruction_set_computer/program_listings.hpp"
#include "minimal_instruction_set_computer/animation.hpp"

struct MinimalInstructionSetComputer : public DemoBase  {
  MinimalInstructionSetComputer()
    : __program(MISC::listing)
    , __acc_index(0)
    , __counter(0)
    , __highlight(0)
  {
  }

private:
  static Value const OFF = 0u;
  static Value const ON = MAX_VALUE / 2;
  static Value const OFF_HIGH = 1u;
  static Value const ON_HIGH = MAX_VALUE;

  static int const COUNTER_COLUMNS = 3;
  static int const WIDTH = N_COLUMNS - COUNTER_COLUMNS;
  static int const HIGHLIGHT_INDEX = COUNTER_COLUMNS + WIDTH / 2;

  static float constexpr MS_ANIM_FRAME = 30.0f;
  static float constexpr MS_ACC_LERP = 20.0f;
  static float constexpr MS_COUNTER_LERP = 50.0f;
  static float constexpr MS_INSTRUCTION = 100.0f;
  static float constexpr MS_RESET = 5000.0f;

  void lerp(int & _from, int _to) {
    float speed = max(float(abs(_to - _from)) / WIDTH, 1.0f);
    _from += (_to < _from ? -1 : 1) * speed;
  }

  void reset() {
    __program.reset();
    __instruction.init(__program.getNextInstruction(), __program.getAccumulator(), __program.getNextValue());
  }

  virtual void __update() {
    int counter = __program.getCounter();
    int ins = __program.getNextInstruction();
    int address = __program.getNextAddress();
    if(__counter != counter) {
      if(__highlight != 0 && counter != __counter + 1 && counter != __counter + 2) {
        lerp(__highlight, 0);
        __frame_ms = MS_ACC_LERP;
      } else {
        lerp(__counter, counter);
        __frame_ms = MS_COUNTER_LERP;
      }
    } else if(MISC::isAccumulatorInstruction(ins) && (__highlight != HIGHLIGHT_INDEX ||__acc_index != address)) {
      if(__highlight != HIGHLIGHT_INDEX) {
        lerp(__highlight, HIGHLIGHT_INDEX);
      } else {
        lerp(__acc_index, address);
      }
      __frame_ms = MS_ACC_LERP;
    } else if(__instruction.animate()) {
      __frame_ms = MS_ANIM_FRAME;
    } else if(__program.next()) {
      __instruction.init(__program.getNextInstruction(), __program.getAccumulator(), __program.getNextValue());
      __frame_ms = MS_INSTRUCTION;
    } else {
      reset();
      __frame_ms = MS_RESET;
    }
  }

  virtual Value __getValue(Index _col, Index _row) const {
    _col = N_COLUMNS - _col - 1;
    int val = 0;
    switch(_col) {
      case 0: val = __counter; break;
      case 1: val = __program[__counter]; break;
      case 2: val = __program[__counter + 1]; break;
      default: {
        int val_idx = int(_col) - COUNTER_COLUMNS;
        val_idx += __acc_index;
        val_idx -= WIDTH / 2;
        val = val_idx >= 0 ? __program[val_idx] : 0;
      } break;
    }

    float b = 1.0f - (_col >= COUNTER_COLUMNS ? abs(float(_col) - float(HIGHLIGHT_INDEX)) / (HIGHLIGHT_INDEX - COUNTER_COLUMNS) : 0.0f);
    Value row_val = (val & (1 << _row) ? Value(ceil(ON * b)) : OFF);

    Value highlight_val = OFF;
    if(_col == __highlight) {
      if(__highlight <= 2) {
        highlight_val = val & (1 << _row) ? ON_HIGH : OFF_HIGH;
      } else if(__instruction.isAnimating()) {
        highlight_val = __instruction[_row] * ON_HIGH + OFF_HIGH;
      } else {
        highlight_val = __program.getAccumulator() & (1 << _row) ? ON_HIGH : OFF_HIGH;
      }
    }

    return min(MAX_VALUE, row_val + highlight_val);
  }

  virtual float __getFrameMS() const {return __frame_ms;}

  MISC::Program __program;
  MISC::AnimationManager<N_ROWS> __instruction;
  int __acc_index;
  int __counter;
  int __highlight;
  float __frame_ms;
};
