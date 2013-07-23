#include "NodDetector.hpp"
#include <limits>
#include <cstdlib>
#include "edk.h"
#include <iostream>

template<class T>
inline T sign(T x) {
    return x > 0 ?  1 :
           x < 0 ? -1 :
                    0;
}

void NodDetector::_fixWindow() {
    DWORD minTime = GetTickCount()-WINDOW_DURATION;
    _GyroReading prevReading;
    for(std::list<_GyroReading>::iterator i=_signalWindow.begin();
         i != _signalWindow.end();++i) {
        if(i->time <= minTime) {

            bool xBaseLine = (std::abs(i->x) <= MAX_BASELINE),
                 yBaseLine = (std::abs(i->y) <= MAX_BASELINE);
            if(i != _signalWindow.begin()) {
                xBaseLine = (xBaseLine && std::abs(prevReading.x) <= MAX_BASELINE);
                yBaseLine = (yBaseLine && std::abs(prevReading.y) <= MAX_BASELINE);
            }
            std::list<_GyroReading>::iterator j = i;
            ++j;
            if(j != _signalWindow.end()) {
                xBaseLine = (xBaseLine && std::abs(j->x) <= MAX_BASELINE);
                yBaseLine = (yBaseLine && std::abs(j->y) <= MAX_BASELINE);
            }
            if(i->time > _lastXPulse && !xBaseLine) {
                _lastXPulse = i->time;
            }
            if(i->time > _lastYPulse && !yBaseLine) {
                _lastYPulse = i->time;
            }
        }
        prevReading = *i;
        if(i->time >= minTime) {
            _signalWindow.erase(_signalWindow.begin(),i);
            break;
        }
    }
}

void NodDetector::read(std::ostream& log) {
    _GyroReading newReading;
    newReading.time = GetTickCount();
    EE_HeadsetGetGyroDelta(0,&newReading.x,&newReading.y);
    if(!_signalWindow.empty()) {
        _GyroReading prev = _signalWindow.back();
        if(prev.x != newReading.x || prev.y != newReading.y) {
            if(prev.time > _lastLog) {
                log << prev.time << ",,,,,,,,,,,," << prev.x << "," << prev.y << std::endl;
            }
            log << newReading.time << ",,,,,,,,,,,," << newReading.x << "," << newReading.y << std::endl;
            _lastLog = newReading.time;
        }
    }
    if(std::abs(newReading.x) > 1 || std::abs(newReading.y) > 1) {
        /*
        if(!_signalWindow.empty()) {
            _GyroReading last = _signalWindow.back();
            newReading.x += last.x;
            newReading.y += last.y;
        }
        */
        //std::cout << "Gyros: x=" << newReading.x
        //          <<       ",y=" << newReading.y << std::endl;
        _signalWindow.push_back(newReading);
    }
}

void NodDetector::getNod(int& x,int& y) {
    _fixWindow();
    if(_signalWindow.empty()) {
        x = y = 0;
        return;
    }
    _GyroReading first = *_signalWindow.begin();
    if(_lastXPulse < first.time && std::abs(first.x) <= MAX_BASELINE) {
        int maxVal = 0;
        DWORD maxValTime;
        for(std::list<_GyroReading>::iterator i=_signalWindow.begin();
             i != _signalWindow.end();++i) {
            if(std::abs(i->x) > std::abs(maxVal)) {
                maxVal = i->x;
                maxValTime = i->time;
            }
        }
        if(std::abs(maxVal) > MIN_INTENSITY) {
            x = sign(maxVal);
            _lastXPulse = maxValTime;
        } else {
            x = 0;
        }
    } else {
        x = 0;
    }
    if(_lastYPulse < first.time && std::abs(first.y) <= MAX_BASELINE) {
        int maxVal = 0;
        DWORD maxValTime;
        for(std::list<_GyroReading>::iterator i=_signalWindow.begin();
             i != _signalWindow.end();++i) {
            if(std::abs(i->y) > std::abs(maxVal)) {
                maxVal = i->y;
                maxValTime = i->time;
            }
        }
        if(std::abs(maxVal) > MIN_INTENSITY) {
            y = sign(maxVal);
            _lastYPulse = maxValTime;
        } else {
            y = 0;
        }
    } else {
        y = 0;
    }
}
