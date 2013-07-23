#ifndef MORSETRANSLATOR_HPP
#define MORSETRANSLATOR_HPP

#include <vector>
#include <string>
#include <fstream>
#include "AutoComplete.h"

class MorseTranslator {
    static void morseSuggestion(std::string start,std::vector<std::pair<std::string,std::string> >& out);

    std::string _text, _currWord, _morseLetter;
    int _replaceBegin,_replaceEnd;
    std::string _replacedWord,_replacedMorse;
    std::vector<std::pair<std::string,std::string> > _suggestions;
    std::vector<std::string> _wordSuggestions;
    unsigned _selectedSuggestion;
    autoCompleteVars *vars;

    std::ofstream rawLog,textLog;

    void _buildWordSuggestions();
    void _add(char c);
public:
    MorseTranslator();
    ~MorseTranslator();

    void add(std::string morse);
    const std::string& text() const {
        return _text;
    }
    const std::string& currWord() const {
        return _currWord;
    }
    const std::string& morseLetter() const {
        return _morseLetter;
    }
    void clear();
    void backspace();
    const std::vector<std::pair<std::string,std::string> >& morseSuggestions() {
        return _suggestions;
    }
    const std::vector<std::string>& wordSuggestions() {
        return _wordSuggestions;
    }
    unsigned selectionIndex() {
        return _selectedSuggestion;
    }
};

#endif
