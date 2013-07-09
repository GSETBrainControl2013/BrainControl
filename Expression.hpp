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

    DWORD time;
    unsigned eyeState;
    EE_ExpressivAlgo_t upperFace,lowerFace;
    float upperFacePwr,lowerFacePwr;
    int headX,headY; // >0 means right/up, <0 means left/down,0 means nothing

    Expression(unsigned _eyeState = NOTHING,
               EE_ExpressivAlgo_t _upperFace = EXP_NEUTRAL,
               float _upperFacePwr = 0,
               EE_ExpressivAlgo_t _lowerFace = EXP_NEUTRAL,
               float _lowerFacePwr = 0) :
                   eyeState(_eyeState),
                   upperFace(_upperFace),
                   lowerFace(_lowerFace),
                   upperFacePwr(_upperFacePwr),
                   lowerFacePwr(_lowerFacePwr) {}
    Expression(EmoStateHandle state);
};

std::ostream& operator<<(std::ostream& os,const Expression& e);

#endif
