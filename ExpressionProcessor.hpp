#ifndef EXPRESSIONPROCESSOR_HPP
#define EXPRESSIONPROCESSOR_HPP

#include "Expression.hpp"
#include "EmoEngine.hpp"

class ExpressionProcessor {
    static const DWORD DECAY_TIME;
    std::queue<Expression> _processed;
    std::vector<std::pair<DWORD,Expression> > _prevExpressions;

public:
    ExpressionProcessor() {}

    void processEvents(EmoEngine& engine);
    void process(const Expression& e);
    bool getExpression(Expression& e);
};

#endif
