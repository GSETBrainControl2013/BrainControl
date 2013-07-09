#include "EmoStateDLL.h"
#include "edk.h"
#include <iostream>
#include "EmoEngine.hpp"
#include "Expression.hpp"
#include "ExpressionProcessor.hpp"
#include "NodDetector.hpp"

int main() {
	EmoEngine engine;
	ExpressionProcessor processor;
	NodDetector nod;

	while(1) {
        nod.read();
        processor.processEvents(engine);

        Expression e;
        while(processor.getExpression(e)) {
            std::cout << "Processed: " << e << std::endl;
        }
        int x,y;
        nod.getNod(x,y);
        if(x > 0) {
            std::cout << "Shake right" << std::endl;
        } else if(x < 0) {
            std::cout << "Shake left" << std::endl;
        }
        if(y > 0) {
            std::cout << "Nod up" << std::endl;
        } else if(y < 0) {
            std::cout << "Nod down" << std::endl;
        }
        std::cout.flush();
		Sleep(1);
	}
}
