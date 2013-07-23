#include "Exp2Morse.hpp"

std::string exp2Morse(Expression inputExp) {
    if (inputExp.event == Expression::LSMIRK) {
        return "-";
    }
    if (inputExp.event == Expression::RSMIRK) {
        return ".";
    }
    if (inputExp.event == Expression::EYEBROW) {
        return " ";
    }
    if (inputExp.event == Expression::SHAKE_RIGHT) {
        return "/";
    }
    if (inputExp.event == Expression::SHAKE_LEFT) {
        return "<";
    }
    if (inputExp.event == Expression::NOD_UP) {
        return "^";
    }
    if (inputExp.event == Expression::NOD_DOWN) {
        return "v";
    }
    if (inputExp.event == Expression::THOUGHT_LIFT) {
        return "@";
    }
    return "";
}
