#ifndef EXPRESSIONPROCESSOR_HPP
#define EXPRESSIONPROCESSOR_HPP

#include "Expression.hpp"
#include "EmoEngine.hpp"
#include "NodDetector.hpp"
#include <fstream>

class ExpressionProcessor {
    struct _Expression {
        Expression upperFace,lowerFace,thought;
    };
    static const DWORD DECAY_TIME;
    static const float MIN_UPPER_PWR,
                       MIN_LOWER_PWR;
    std::queue<Expression> _processed;
    std::vector<std::pair<DWORD,_Expression> > _prevExpressions;
    NodDetector _nod;
    std::ofstream log;

    void _readNod();
public:
    ExpressionProcessor() : log("logs/raw.csv",std::ios_base::out|std::ios_base::trunc) {
        log << "Time (ms),Lower,LowerPwr,Upper,UpperPwr,Thought,ThoughtPwr,Blink,LWink,RWink,LLook,RLook,GyroX,GyroY" << std::endl;
    }

    void processEvents(EmoEngine& engine);
    void process(EmoStateHandle state);
    bool getExpression(Expression& e);
};

#endif
