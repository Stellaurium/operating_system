//
// Created by stellaura on 04/03/24.
//

#ifndef _LOGIC_FLIP_FLOP__H_
#define _LOGIC_FLIP_FLOP__H_


#include "logic_basic_.hpp"

// FlipFlop触发器 (D 触发器)
// Q* = D
class DFlipFlop : public Device {
  public:
    DFlipFlop(Wire *in_, Wire *out_) : in(in_), out(out_), pre_in(*in_), pre_out(*out_) {}
    void update() override {
        out->signal = in->signal;
        if (*in == pre_in && *out == pre_out) {
            is_state_changed = false;
        } else {
            is_state_changed = true;
        }
        pre_in = *in;
        pre_out = *out;
    }

  private:
    Wire *in;
    Wire *out;
    Wire pre_in;
    Wire pre_out;
};



#endif //_LOGIC_FLIP_FLOP__H_
