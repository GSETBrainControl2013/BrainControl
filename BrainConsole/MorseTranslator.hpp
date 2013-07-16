#ifndef MORSETRANSLATOR_HPP
#define MORSETRANSLATOR_HPP

#include <vector>
#include <string>

class MorseTranslator {
    static void morseSuggestion(std::string start,std::vector<std::pair<std::string,std::string> >& out);

    std::string _text;
    std::string _morseLetter;
    std::vector<std::pair<std::string,std::string> > _suggestions;
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
    const std::vector<std::pair<std::string,std::string> >& suggestions() {
        return _suggestions;
    }
};

#endif
