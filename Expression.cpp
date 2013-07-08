#include "Expression.hpp"
#include <sstream>

Expression::Expression(EmoStateHandle state) {
    upperFace = ES_ExpressivGetUpperFaceAction(state);
    lowerFace = ES_ExpressivGetLowerFaceAction(state);
    upperFacePwr = ES_ExpressivGetUpperFaceActionPower(state);
    lowerFacePwr = ES_ExpressivGetLowerFaceActionPower(state);

    eyeState = 0;
    if(ES_ExpressivIsBlink(state)) {
        eyeState |= Expression::BLINK;
    }
    if(ES_ExpressivIsLeftWink(state)) {
        eyeState |= Expression::LWINK;
    }
    if(ES_ExpressivIsRightWink(state)) {
        eyeState |= Expression::RWINK;
    }
    if(ES_ExpressivIsLookingRight(state)) {
        eyeState |= Expression::RLOOK;
    }
    if(ES_ExpressivIsLookingLeft(state)) {
        eyeState |= Expression::LLOOK;
    }
}

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
    if(e.upperFacePwr > 0) {
		switch (e.upperFace) {
			case EXP_EYEBROW:	str << "eyebrow";	break;
			case EXP_FURROW:    str << "furrow";  break;
			default:			break;
		}
		str << (int)(e.upperFacePwr*100) << ",";
    }
    if(e.lowerFacePwr > 0) {
        switch (e.lowerFace) {
			case EXP_CLENCH:	str << "Clench";	break;
			case EXP_SMILE:		str << "Smile";	break;
			case EXP_LAUGH:     str << "Laugh";  break;
			case EXP_SMIRK_LEFT:  str << "LSmirk"; break;
			case EXP_SMIRK_RIGHT: str << "RSmirk"; break;
			default:			break;
		}
		str << (int)(e.lowerFacePwr*100) << ",";
    }
    std::string outStr = str.str();
    if(outStr.empty()) {
        os << "neutral";
    } else {
        os << outStr.substr(0,outStr.length()-1);
    }
    return os;
}
