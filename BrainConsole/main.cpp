#include "MorseTranslator.hpp"
#include <iostream>

int main() {
    MorseTranslator translator;
    std::string str = ".. / -.-. .- -. .----. - / .-.. . - / -.-- --- ..- / -.. --- / - .... .- - --..-- / .--- .. -- .-.-.- ";
    for(size_t i=0;i<str.length();++i) {
        std::string morse = str.substr(i,1);
        translator.add(morse);
        std::cout << translator.text() << "|" << translator.morseLetter() << std::endl;
    }
    std::cin.get();
    return 0;
}
