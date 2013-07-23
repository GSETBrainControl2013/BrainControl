#include "MorseTranslator.hpp"
#include <map>
#include <iostream>
#include <algorithm>
#include <string>

const size_t _MORSE_CHARS = 54;

std::string _morse[_MORSE_CHARS][2] = {
    { "A",".-" },
    { "B","-..." },
    { "C","-.-." },
    { "D","-.." },
    { "E","." },
    { "F","..-." },
    { "G","--." },
    { "H","...." },
    { "I",".." },
    { "J",".---" },
    { "K","-.-" },
    { "L",".-.." },
    { "M","--" },
    { "N","-." },
    { "O","---" },
    { "P",".--." },
    { "Q","--.-" },
    { "R",".-." },
    { "S","..." },
    { "T","-" },
    { "U","..-" },
    { "V","...-" },
    { "W",".--" },
    { "X","-..-" },
    { "Y","-.--" },
    { "Z","--.." },
    { "0","-----" },
    { "1",".----" },
    { "2","..---" },
    { "3","...--" },
    { "4","....-" },
    { "5","....." },
    { "6","-...." },
    { "7","--..." },
    { "8","---.." },
    { "9","----." },
    { ".",".-.-.-" },
    { ",","--..--" },
    { "?","..--.." },
    { "'",".----." },
    { "!","-.-.--" },
    { "/","-..-." },
    { "(","-.--." },
    { ")","-.--.-" },
    { "&",".-..." },
    { ":","---..." },
    { ";","-.-.-." },
    { "=","-...-" },
    { "+",".-.-." },
    { "-","-....-" },
    { "_","..--.-" },
    {"\"",".-..-." },
    { "$","...-..-" },
    { "@",".--.-." }
};

bool _morseCmp(const std::pair<std::string,std::string>& a,
               const std::pair<std::string,std::string>& b) {
    return a.first.length() < b.first.length() || a.second < b.second;
}

void MorseTranslator::morseSuggestion(std::string start,std::vector<std::pair<std::string,std::string> >& out) {
    out.clear();
    out.reserve(_MORSE_CHARS);
    for(size_t i=0;i<_MORSE_CHARS;++i) {
        if(_morse[i][1].compare(0, start.length(), start) == 0) { // begins with
            out.push_back(std::make_pair(_morse[i][1],_morse[i][0]));
        }
    }
    // Bubble sort, because std::sort doesn't like me.
    bool swapped = true;
    size_t size = out.size();
    for(size_t i=0;i<out.size()&&swapped;++i) {
        swapped = false;
        for(size_t j=0;j+2+i<out.size();++j) {
            if(out[j].first.length() > out[j+1].first.length() ||
               (out[j].first.length() == out[j+1].first.length() && out[j].second > out[j+1].second)) {
                std::pair<std::string,std::string> tmp = out[j];
                out[j] = out[j+1];
                out[j+1] = tmp;
                swapped = true;
            }
        }
    }
}

MorseTranslator::MorseTranslator() : rawLog("logs/morse.txt",std::ios_base::out|std::ios_base::trunc),
                                     textLog("logs/text.txt",std::ios_base::out|std::ios_base::trunc),
                                     _selectedSuggestion(0),_replaceEnd(-1) {
    add("");
}

void MorseTranslator::_buildWordSuggestions() {
    _wordSuggestions.clear();
    _wordSuggestions.push_back(_currWord);
    _wordSuggestions.push_back("Hi!");
}

std::map<std::string,std::string> morseToTxt;

void MorseTranslator::_add(char c) {
    rawLog << c;
    if(c == '>') {
        c = '/';
    }
    if(c == ' ') {
        //if(!_suggestions.empty()) {
            std::string currLetter = _suggestions[0].first;
            std::map<std::string,
                     std::string>::iterator character = morseToTxt.find(currLetter);
            if(character != morseToTxt.end()) {
                _currWord += character->second;
                textLog << character->second;
            }
        //}
        _morseLetter.clear();
    }
    if(c == '/') {
        std::string replacement = _wordSuggestions[_selectedSuggestion];
        _replacedWord = _currWord;
        _replacedMorse = _morseLetter;
        _replaceBegin = _text.size();
        _replaceEnd = _replaceBegin+replacement.length()+1;
        _text += replacement + " ";
        _currWord = _morseLetter = "";
        _selectedSuggestion = 0;
    }
    if(c == '.' || c == '-') {
        _morseLetter += c;
    }
    if(c == '<') {
        backspace();
    }
    if(c == '^') {
        --_selectedSuggestion;
        _selectedSuggestion %= _wordSuggestions.size();
    }
    if(c == 'v') {
        ++_selectedSuggestion;
        _selectedSuggestion %= _wordSuggestions.size();
    }
    morseSuggestion(_morseLetter,_suggestions);
    _buildWordSuggestions();
}

void MorseTranslator::add(std::string morse) {
    if(morseToTxt.empty()) {
        for(size_t i=0;i<_MORSE_CHARS;++i) {
            //std::cout << "'" << _morse[i][1] << "':'" << _morse[i][0] << "'" << std::endl;
            morseToTxt[_morse[i][1]] = _morse[i][0];
        }
    }
    for(size_t i=0;i<morse.size();++i) {
        _add(morse[i]);
    }
    morseSuggestion(_morseLetter,_suggestions);
    _buildWordSuggestions();
}

void MorseTranslator::clear() {
    _text.clear();
    add("");
}

void MorseTranslator::backspace() {
    if(!_morseLetter.empty()) {
        _morseLetter = _morseLetter.substr(0,_morseLetter.size()-1);
    } else if(!_currWord.empty()) {
        _currWord = _currWord.substr(0,_currWord.size()-1);
    } else if(_text.length() == _replaceEnd) {
        _text.erase(_replaceBegin);
        _currWord = _replacedWord;
        _morseLetter = _replacedMorse;
        _replaceEnd = -1;
    } else if(!_text.empty()) {
        _text = _text.substr(0,_text.size()-1);
    }
}
