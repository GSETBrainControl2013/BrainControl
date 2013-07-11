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
    return "";
}
