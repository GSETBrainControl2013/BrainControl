#include "Expression.hpp"
#include <sstream>

std::ostream& operator<<(std::ostream& os,const Expression& e) {
    std::ostringstream str;
    if(e.eyeState & Expression::BLINK) {
        str << "Blink,";
    }
    if(e.eyeState & Expression::LWINK) {
        str << "LWink,";
    }
    if(e.eyeState & Expression::RWINK) {
        str << "RWink,";
    }
    if(e.eyeState & Expression::RLOOK) {
        str << "RLook,";
    }
    if(e.eyeState & Expression::LLOOK) {
        str << "LLook,";
    }
    if(e.power > 0) {
        switch (e.event) {
            case Expression::EYEBROW:      str << "Eyebrow";     break;
            case Expression::FURROW:       str << "Furrow";      break;
            case Expression::SMILE:        str << "Smile";       break;
            case Expression::CLENCH:       str << "Clench";      break;
            case Expression::LAUGH:        str << "Laugh";       break;
            case Expression::LSMIRK:       str << "LSmirk";      break;
            case Expression::RSMIRK:       str << "RSmirk";      break;
            case Expression::NOD_UP:       str << "NodUp";       break;
            case Expression::NOD_DOWN:     str << "NodDown";     break;
            case Expression::SHAKE_LEFT:   str << "ShakeLeft";   break;
            case Expression::SHAKE_RIGHT:  str << "ShakeRight";  break;
            case Expression::THOUGHT_LIFT: str << "ThoughtLift"; break;
            case Expression::THOUGHT_DROP: str << "ThoughtDrop"; break;
            default:            break;
        }
        str << (int)(e.power*100) << ",";
    }
    std::string outStr = str.str();
    if(outStr.empty()) {
        os << "neutral";
    } else {
        os << outStr.substr(0,outStr.length()-1);
    }
    return os;
}

Expression::Event Expression::toEvent(EE_ExpressivAlgo_t exp) {
    switch(exp) {
    case EXP_EYEBROW:
        return EYEBROW;
    case EXP_FURROW:
        return FURROW;
    case EXP_SMILE:
        return SMILE;
    case EXP_CLENCH:
        return CLENCH;
    case EXP_LAUGH:
        return LAUGH;
    case EXP_SMIRK_LEFT:
        return LSMIRK;
    case EXP_SMIRK_RIGHT:
        return RSMIRK;
    default:
        break;
    }
    return NEUTRAL;
}

Expression::Event Expression::toEvent(EE_CognitivAction_t cog) {
    switch(cog) {
    case COG_LIFT:
        return THOUGHT_LIFT;
    case COG_DROP:
        return THOUGHT_DROP;
    default:
        break;
    }
    return NEUTRAL;
}
