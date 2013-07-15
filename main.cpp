#include "EmoStateDLL.h"
#include "edk.h"
#include <iostream>
#include "EmoEngine.hpp"
#include "Expression.hpp"
#include "ExpressionProcessor.hpp"
#include "NodDetector.hpp"
#include "Exp2Morse.hpp"
#include "MorseTranslator.hpp"
#include <sstream>

int main() {
	EmoEngine engine;
	ExpressionProcessor processor;
    MorseTranslator translator;
	//NodDetector nod;

    std::string txt = "";
	while(1) {
        //nod.read();
        processor.processEvents(engine);

        Expression e;
        while(processor.getExpression(e)) {
            std::string morse = exp2Morse(e);
            if(!morse.empty()) {
                translator.add(exp2Morse(e));
                std::stringstream newTxt;
                newTxt << translator.text() << "|" << translator.morseLetter();
                if(newTxt.str() != txt) {
                    txt = newTxt.str();
                    std::cout << txt << std::endl;
                }
            }
        }
        std::cout.flush();
		Sleep(1);
	}
}
