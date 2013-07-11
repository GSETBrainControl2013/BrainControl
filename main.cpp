#include "EmoStateDLL.h"
#include "edk.h"
#include <iostream>
#include "EmoEngine.hpp"
#include "Expression.hpp"
#include "ExpressionProcessor.hpp"
#include "NodDetector.hpp"
#include "Exp2Morse.hpp"

int main() {
	EmoEngine engine;
	ExpressionProcessor processor;
	//NodDetector nod;

	while(1) {
        //nod.read();
        processor.processEvents(engine);

        Expression e;
        while(processor.getExpression(e)) {
            std::cout << exp2Morse(e);
            //std::cout << e << std::endl;
        }
        std::cout.flush();
		Sleep(1);
	}
}
