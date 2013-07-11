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
    EmoEngine();
    ~EmoEngine();
    bool retrieveEvent();
    EmoEngineEventHandle event();

    EmoStateHandle eventState();
};

#endif
