#include "ExpressionProcessor.hpp"

const DWORD ExpressionProcessor::DECAY_TIME = 0.25*1000;
const float ExpressionProcessor::MIN_UPPER_PWR = 0.30,
            ExpressionProcessor::MIN_LOWER_PWR = 0.20;

void ExpressionProcessor::_readNod() {
    _nod.read(log);
    int x,y;
    _nod.getNod(x,y);
    DWORD currTime = GetTickCount();
    Expression e;
    e.time = currTime;
    e.eyeState = 0;
    e.power = 1;
    if(x > 0) {
        e.event = Expression::SHAKE_RIGHT;
        _processed.push(e);
    } else if(x < 0) {
        e.event = Expression::SHAKE_LEFT;
        _processed.push(e);
    }
    if(y > 0) {
        e.event = Expression::NOD_UP;
        _processed.push(e);
    } else if(y < 0) {
        e.event = Expression::NOD_DOWN;
        _processed.push(e);
    }
}

void ExpressionProcessor::process(EmoStateHandle state) {
    _Expression e;
    DWORD currTime    = GetTickCount();
    e.upperFace.time  = e.lowerFace.time = currTime;
    e.upperFace.event = Expression::toEvent(ES_ExpressivGetUpperFaceAction(state));
    e.lowerFace.event = Expression::toEvent(ES_ExpressivGetLowerFaceAction(state));
    e.upperFace.power = ES_ExpressivGetUpperFaceActionPower(state);
    e.lowerFace.power = ES_ExpressivGetLowerFaceActionPower(state);
    e.thought.event   = Expression::toEvent(ES_CognitivGetCurrentAction(state));
    e.thought.power   = ES_CognitivGetCurrentActionPower(state);

    e.lowerFace.eyeState = e.upperFace.eyeState = 0;
    if(ES_ExpressivIsBlink(state)) {
        e.lowerFace.eyeState |= Expression::BLINK;
    }
    if(ES_ExpressivIsLeftWink(state)) {
        e.lowerFace.eyeState |= Expression::LWINK;
    }
    if(ES_ExpressivIsRightWink(state)) {
        e.lowerFace.eyeState |= Expression::RWINK;
    }
    if(ES_ExpressivIsLookingRight(state)) {
        e.lowerFace.eyeState |= Expression::RLOOK;
    }
    if(ES_ExpressivIsLookingLeft(state)) {
        e.lowerFace.eyeState |= Expression::LLOOK;
    }

    log << currTime << "," << e.lowerFace.event << "," << e.lowerFace.power << ","
                           << e.upperFace.event << "," << e.upperFace.power << ","
                           << e.thought.event   << "," << e.thought.power   << ","
                           << !!(e.lowerFace.eyeState & Expression::BLINK) << ","
                           << !!(e.lowerFace.eyeState & Expression::LWINK) << ","
                           << !!(e.lowerFace.eyeState & Expression::RWINK) << ","
                           << !!(e.lowerFace.eyeState & Expression::RLOOK) << ","
                           << !!(e.lowerFace.eyeState & Expression::LLOOK);
    //std::cout << "Processing: " << e << std::endl;
    _Expression toAdd = e;
    if(!_prevExpressions.empty()) {
        e.lowerFace.eyeState &= ~(_prevExpressions.end()-1)->second.lowerFace.eyeState;
    }
    if(e.upperFace.power < MIN_UPPER_PWR) {
        e.upperFace.event = Expression::NEUTRAL;
        e.upperFace.power = 0;
    }
    if(e.lowerFace.power < MIN_LOWER_PWR) {
        e.lowerFace.event = Expression::NEUTRAL;
        e.lowerFace.power = 0;
    }
    for(std::vector<std::pair<DWORD,_Expression> >::iterator i=_prevExpressions.begin();
         i != _prevExpressions.end();++i) {
        if(currTime-i->first > DECAY_TIME) {
            _prevExpressions.erase(i--);
        } else {
            if(i->second.upperFace.event == e.upperFace.event) {
                e.upperFace.event = Expression::NEUTRAL;
                e.upperFace.power = 0;
            }
            if(i->second.lowerFace.event == e.lowerFace.event) {
                e.lowerFace.event = Expression::NEUTRAL;
                e.lowerFace.power = 0;
            }
            if(i->second.thought.event == e.thought.event) {
                e.thought.event = Expression::NEUTRAL;
                e.thought.power = 0;
            }
        }
    }
    _prevExpressions.push_back(std::make_pair(currTime,toAdd));
    if(e.lowerFace.event != Expression::NEUTRAL ||
       e.lowerFace.eyeState != Expression::NOTHING) {
        _processed.push(e.lowerFace);
    }
    if(e.upperFace.event != Expression::NEUTRAL) {
        _processed.push(e.upperFace);
    }
    if(e.thought.event != Expression::NEUTRAL) {
        _processed.push(e.thought);
    }
}

bool ExpressionProcessor::getExpression(Expression& e) {
    if(_processed.empty()) {
        return false;
    }
    e = _processed.front();
    std::cout << "Expression: " << e << std::endl;
    _processed.pop();
    return true;
}


void ExpressionProcessor::processEvents(EmoEngine& engine) {
    bool nodRead = false;
    while(engine.retrieveEvent()) {
        EmoEngineEventHandle event = engine.event();

        unsigned userID;
        EE_Event_t eventType = EE_EmoEngineEventGetType(event);
        EE_EmoEngineEventGetUserId(event, &userID);

        if(eventType == EE_EmoStateUpdated) {
            process(engine.eventState());
        } else {
            log << GetTickCount() << ",,,,,,,,,,,";
        }
        log << std::endl;
        _readNod();
    }
    if(!nodRead) {
        _readNod();
    }
}
