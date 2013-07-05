#include "EmoStateDLL.h"
#include "edk.h"
#include <windows.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

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

void sendExpressivAnimation(std::ostream& out, EmoStateHandle eState) {

    std::ostringstream output;

	EE_ExpressivAlgo_t upperFaceType = ES_ExpressivGetUpperFaceAction(eState);
	EE_ExpressivAlgo_t lowerFaceType = ES_ExpressivGetLowerFaceAction(eState);

	float upperFaceAmp = ES_ExpressivGetUpperFaceActionPower(eState);
	float lowerFaceAmp = ES_ExpressivGetLowerFaceActionPower(eState);

	if (ES_ExpressivIsBlink(eState)) {
		output << "B,";
	}

	if (ES_ExpressivIsLeftWink(eState)) {
		output << "l,";
	}

	if (ES_ExpressivIsRightWink(eState)) {
		output << "r,";
	}

	if (ES_ExpressivIsLookingRight(eState)) {
		output << "R,";
	}

	if (ES_ExpressivIsLookingLeft(eState)) {
		output << "L,";
	}

	if (upperFaceAmp > 0.0) {
		switch (upperFaceType) {
			case EXP_EYEBROW:	output << "b";	break;
			case EXP_FURROW:    output << "f";  break;
			default:			break;
		}
		output << static_cast<int>(upperFaceAmp*100.0f) << ",";
	}

	if (lowerFaceAmp > 0.0) {
		switch (lowerFaceType) {
			case EXP_CLENCH:	output << "G";	break;
			case EXP_SMILE:		output << "S";	break;
			case EXP_LAUGH:     output << "H";  break;
			case EXP_SMIRK_LEFT:  output << "sl"; break;
			case EXP_SMIRK_RIGHT: output << "sr"; break;
			default:			break;
		}
		output << static_cast<int>(lowerFaceAmp*100.0f) << ",";
	}

	std::string outString = output.str();

	// Remove the last comma
	if (outString.length()) {
		outString.resize(outString.length()-1);
	}

	if (!outString.length()) {
		outString = std::string("neutral");
	}

	out << outString;
}

int main() {
	EmoEngine engine;

	while(1) {
        while(engine.retrieveEvent()) {
            EmoEngineEventHandle event = engine.event();

            unsigned userID;
            EE_Event_t eventType = EE_EmoEngineEventGetType(event);
			EE_EmoEngineEventGetUserId(event, &userID);

			if(eventType == EE_EmoStateUpdated) {
                EmoStateHandle state = engine.eventState();
                sendExpressivAnimation(std::cout, state);
                std::cout << std::endl;
			}
        }

		Sleep(1);
	}
}
