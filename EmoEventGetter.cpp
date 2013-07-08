#include "EmoStateDLL.h"
#include "edk.h"
#include <windows.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <queue>

class EmoEngine {
	EmoEngineEventHandle _event;
	EmoStateHandle _state;
	int _stateCode;

public:
    EmoEngine() {
        if (EE_EngineConnect() != EDK_OK) {
            throw std::runtime_error("Emotiv Engine start up failed.");
        }
        _stateCode = EDK_OK;
        _event = EE_EmoEngineEventCreate();
        _state = EE_EmoStateCreate();
    }
    ~EmoEngine() {
        EE_EngineDisconnect();
        EE_EmoStateFree(_state);
        EE_EmoEngineEventFree(_event);
    }
    bool retrieveEvent() {
        _stateCode = EE_EngineGetNextEvent(_event);
        if(_stateCode == EDK_NO_EVENT) {
            return false;
        } else if(_stateCode == EDK_OK) {
            return true;
        }
		throw std::runtime_error("Internal error in Emotiv Engine!");
    }
    EmoEngineEventHandle event() {
        return _event;
    }

    EmoStateHandle eventState() {
        EE_EmoEngineEventGetEmoState(_event,_state);
        return _state;
    }
};

struct Expression {
    enum EyeState {
        NOTHING = 0,
        BLINK   = 1<<0,
        LWINK   = 1<<1,
        RWINK   = 1<<2,
        LLOOK   = 1<<3,
        RLOOK   = 1<<4
    };

    unsigned eyeState;
    EE_ExpressivAlgo_t upperFace,lowerFace;
    float upperFacePwr,lowerFacePwr;

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
};

std::ostream& operator<<(std::ostream& os,const Expression& e) {
    std::ostringstream str;
    if(e.eyeState & Expression::BLINK) {
        str << "B,";
    }
    if(e.eyeState & Expression::LWINK) {
        str << "l,";
    }
    if(e.eyeState & Expression::RWINK) {
        str << "r,";
    }
    if(e.eyeState & Expression::RLOOK) {
        str << "R,";
    }
    if(e.eyeState & Expression::LLOOK) {
        str << "L,";
    }
    if(e.upperFacePwr > 0) {
		switch (e.upperFace) {
			case EXP_EYEBROW:	str << "b";	break;
			case EXP_FURROW:    str << "f";  break;
			default:			break;
		}
		str << (int)(e.upperFacePwr*100) << ",";
    }
    if(e.lowerFacePwr > 0) {
        switch (e.lowerFace) {
			case EXP_CLENCH:	str << "G";	break;
			case EXP_SMILE:		str << "S";	break;
			case EXP_LAUGH:     str << "H";  break;
			case EXP_SMIRK_LEFT:  str << "sl"; break;
			case EXP_SMIRK_RIGHT: str << "sr"; break;
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

Expression extractExpression(EmoStateHandle state) {
    Expression ret;
    ret.upperFace = ES_ExpressivGetUpperFaceAction(state);
    ret.lowerFace = ES_ExpressivGetUpperFaceAction(state);
    ret.upperFacePwr = ES_ExpressivGetUpperFaceActionPower(state);
    ret.lowerFacePwr = ES_ExpressivGetLowerFaceActionPower(state);

    if(ES_ExpressivIsBlink(state)) {
        ret.eyeState |= Expression::BLINK;
    }
    if(ES_ExpressivIsLeftWink(state)) {
        ret.eyeState |= Expression::LWINK;
    }
    if(ES_ExpressivIsRightWink(state)) {
        ret.eyeState |= Expression::RWINK;
    }
    if(ES_ExpressivIsLookingRight(state)) {
        ret.eyeState |= Expression::RLOOK;
    }
    if(ES_ExpressivIsLookingLeft(state)) {
        ret.eyeState |= Expression::LLOOK;
    }
    return ret;
}

class ExpressionProcessor {
    static const float MIN_UPPER_PWR,
                       MIN_LOWER_PWR;
    static const DWORD DECAY_TIME;
    std::queue<Expression> _processed;
    std::vector<std::pair<DWORD,Expression> > _prevExpressions;

public:
    ExpressionProcessor() {}

    void process(const Expression& e) {
        DWORD currTime = GetTickCount();
        Expression processed = e;
        if(processed.upperFacePwr < MIN_UPPER_PWR) {
            processed.upperFacePwr = 0;
            processed.upperFace = EXP_NEUTRAL;
        }
        if(processed.lowerFacePwr < MIN_UPPER_PWR) {
            processed.upperFacePwr = 0;
            processed.upperFace = EXP_NEUTRAL;
        }
        _prevExpressions.push_back(std::make_pair(currTime,processed));
        processed.eyeState &= ~(_prevExpressions.end()-1)->second.eyeState;
        for(std::vector<std::pair<DWORD,Expression> >::iterator i=_prevExpressions.begin();
             i != _prevExpressions.end();++i) {
            if(i->first-currTime > DECAY_TIME) {
                _prevExpressions.erase(i--);
            } else {
                if(i->second.upperFace == processed.upperFace) {
                    processed.upperFace = EXP_NEUTRAL;
                    processed.upperFacePwr = 0;
                }
                if(i->second.lowerFace == processed.lowerFace) {
                    processed.lowerFace = EXP_NEUTRAL;
                    processed.lowerFacePwr = 0;
                }
            }
        }
        if(processed.upperFace != EXP_NEUTRAL || processed.lowerFace != EXP_NEUTRAL ||
           processed.eyeState != 0) {
            _processed.push(processed);
        }
    }
    bool getExpression(Expression& e) {
        if(_processed.empty()) {
            return false;
        }
        e = _processed.front();
        _processed.pop();
        return true;
    }
};
const float ExpressionProcessor::MIN_UPPER_PWR = 0.5,
            ExpressionProcessor::MIN_LOWER_PWR = 0.2;
const DWORD ExpressionProcessor::DECAY_TIME    = 0.5*1000;

void pumpEvents(EmoEngine& engine,ExpressionProcessor& processor) {
    while(engine.retrieveEvent()) {
        EmoEngineEventHandle event = engine.event();

        unsigned userID;
        EE_Event_t eventType = EE_EmoEngineEventGetType(event);
        EE_EmoEngineEventGetUserId(event, &userID);

        if(eventType == EE_EmoStateUpdated) {
            EmoStateHandle state = engine.eventState();
            processor.process(extractExpression(state));
        }
    }
}

int main() {

	EmoEngine engine;
	ExpressionProcessor processor;

	while(1) {
        pumpEvents(engine,processor);

        Expression e;
        while(processor.getExpression(e)) {
            std::cout << e << std::endl;
        }
		Sleep(1);
	}
}
