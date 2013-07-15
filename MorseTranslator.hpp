#ifndef MORSETRANSLATOR_HPP
#define MORSETRANSLATOR_HPP

#include <string>

class MorseTranslator {
    std::string _text;
    std::string _morseLetter;
public:
    void add(std::string morse);
    const std::string& text() const {
        return _text;
    }
    const std::string& morseLetter() const {
        return _morseLetter;
    }
    void clear();
    void backspace();
};

#endif
