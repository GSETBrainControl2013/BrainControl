#include "ExpressionProcessor.hpp"

const float ExpressionProcessor::MIN_UPPER_PWR = 0,
            ExpressionProcessor::MIN_LOWER_PWR = 0;
const DWORD ExpressionProcessor::DECAY_TIME    = 0.5*1000;

void ExpressionProcessor::process(const Expression& e) {
    //std::cout << "Processing: " << e << std::endl;
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
    Expression toAdd = processed;
    if(!_prevExpressions.empty()) {
        processed.eyeState &= ~(_prevExpressions.end()-1)->second.eyeState;
    }
    for(std::vector<std::pair<DWORD,Expression> >::iterator i=_prevExpressions.begin();
         i != _prevExpressions.end();++i) {
        if(currTime-i->first > DECAY_TIME) {
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
    _prevExpressions.push_back(std::make_pair(currTime,toAdd));
    if(processed.upperFace != EXP_NEUTRAL || processed.lowerFace != EXP_NEUTRAL ||
       processed.eyeState != Expression::NOTHING) {
        _processed.push(processed);
    }
}

bool ExpressionProcessor::getExpression(Expression& e) {
    if(_processed.empty()) {
        return false;
    }
    e = _processed.front();
    _processed.pop();
    return true;
}


void ExpressionProcessor::processEvents(EmoEngine& engine) {
    while(engine.retrieveEvent()) {
        EmoEngineEventHandle event = engine.event();

        unsigned userID;
        EE_Event_t eventType = EE_EmoEngineEventGetType(event);
        EE_EmoEngineEventGetUserId(event, &userID);

        if(eventType == EE_EmoStateUpdated) {
            EmoStateHandle state = engine.eventState();
            process(Expression(state));
        }
    }
}
