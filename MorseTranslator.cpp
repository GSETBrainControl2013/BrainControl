#include "MorseTranslator.hpp"
#include <map>
#include <iostream>

const size_t _MORSE_CHARS = 54;

std::string _morse[_MORSE_CHARS][2] = {
    { "A",".-" },
    { "B","-..." },
    { "C","-.-." },
    { "D","-.." },
    { "E","." },
    { "F","..-." },
    { "G","__." },
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

std::map<std::string,std::string> morseToTxt;

void MorseTranslator::add(std::string morse) {
    if(morseToTxt.empty()) {
        for(size_t i=0;i<_MORSE_CHARS;++i) {
            //std::cout << "'" << _morse[i][1] << "':'" << _morse[i][0] << "'" << std::endl;
            morseToTxt[_morse[i][1]] = _morse[i][0];
        }
    }
    _morseLetter += morse;
    std::string currLetter = "";
    int bspCount = 0;
    for(size_t i=0;i<_morseLetter.size();++i) {
        char c = _morseLetter[i];
        if(c == ' ' || c == '/') {
            std::map<std::string,
                     std::string>::iterator character = morseToTxt.find(currLetter);
            if(character != morseToTxt.end()) {
                _text += character->second;
            }
            currLetter.clear();
        }
        if(c == '/') {
            _text += ' ';
        }
        if(c == '.' || c == '-') {
            currLetter += c;
        }
        if(c == '<') {
            ++bspCount;
        }
    }
    _morseLetter = currLetter;
    for(int i=0;i<bspCount;++i) {
        backspace();
    }
}

void MorseTranslator::clear() {
    _text.clear();
}

void MorseTranslator::backspace() {
    if(!_morseLetter.empty()) {
        _morseLetter = _morseLetter.substr(0,_morseLetter.size()-1);
    } else if(!_text.empty()) {
        _text = _text.substr(0,_text.size()-1);
    }
}
