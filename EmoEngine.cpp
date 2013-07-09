#include "EmoEngine.hpp"

EmoEngine::EmoEngine() {
    if (EE_EngineConnect() != EDK_OK) {
        throw std::runtime_error("Emotiv Engine start up failed.");
    }
    _stateCode = EDK_OK;
    _event = EE_EmoEngineEventCreate();
    _state = EE_EmoStateCreate();
}

EmoEngine::~EmoEngine() {
    EE_EngineDisconnect();
    EE_EmoStateFree(_state);
    EE_EmoEngineEventFree(_event);
}

 bool EmoEngine::retrieveEvent() {
    _stateCode = EE_EngineGetNextEvent(_event);
    if(_stateCode == EDK_NO_EVENT) {
        return false;
    } else if(_stateCode == EDK_OK) {
        return true;
    }
    throw std::runtime_error("Internal error in Emotiv Engine!");
}

EmoEngineEventHandle EmoEngine::event() {
    return _event;
}

EmoStateHandle EmoEngine::eventState() {
    EE_EmoEngineEventGetEmoState(_event,_state);
    return _state;
}
