#ifndef EXPRESSIONPROCESSOR_HPP
#define EXPRESSIONPROCESSOR_HPP

#include "Expression.hpp"
#include "EmoEngine.hpp"
#include "NodDetector.hpp"

class ExpressionProcessor {
    struct _Expression {
        Expression upperFace,lowerFace;
    };
    static const DWORD DECAY_TIME;
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
