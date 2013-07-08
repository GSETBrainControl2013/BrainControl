#include "EmoStateDLL.h"
#include "edk.h"
#include <iostream>
#include "EmoEngine.hpp"
#include "Expression.hpp"
#include "ExpressionProcessor.hpp"

int main() {
	EmoEngine engine;
	ExpressionProcessor processor;

	while(1) {
        processor.processEvents(engine);

        Expression e;
        while(processor.getExpression(e)) {
            std::cout << "Processed: " << e << std::endl;
        }
		Sleep(1);
	}
}
