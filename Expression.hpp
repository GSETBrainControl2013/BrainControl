#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "edk.h"
#include <windows.h>
#include <iostream>

struct Expression {
    enum EyeState {
        NOTHING = 0,
        BLINK   = 1<<0,
        LWINK   = 1<<1,
        RWINK   = 1<<2,
        LLOOK   = 1<<3,
        RLOOK   = 1<<4
    };
    enum Event {
		NEUTRAL = 0,
		EYEBROW,
		FURROW,
        SMILE,
		CLENCH,
		LAUGH,
		LSMIRK,
		RSMIRK,
		NOD_UP,
		NOD_DOWN,
		SHAKE_LEFT,
		SHAKE_RIGHT
    };

    DWORD time;
    unsigned eyeState;
    Event event;
    float power;

    static Event toEvent(EE_ExpressivAlgo_t exp);
};

std::ostream& operator<<(std::ostream& os,const Expression& e);

#endif
