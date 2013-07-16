#ifndef EXPRESSIONPROCESSOR_HPP
#define EXPRESSIONPROCESSOR_HPP

#include "Expression.hpp"
#include "EmoEngine.hpp"
#include "NodDetector.hpp"

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

    void _readNod();
public:
    ExpressionProcessor() {}

    void processEvents(EmoEngine& engine);
    void process(EmoStateHandle state);
    bool getExpression(Expression& e);
};

#endif
