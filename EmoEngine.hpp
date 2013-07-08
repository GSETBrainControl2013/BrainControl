#ifndef EMOENGINE_HPP
#define EMOENGINE_HPP

#include "edk.h"
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

#endif
