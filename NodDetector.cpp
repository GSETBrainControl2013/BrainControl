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
    for(std::list<_GyroReading>::iterator i=_signalWindow.begin();
         i != _signalWindow.end();++i) {
        if(i->time > minTime) {
            _signalWindow.erase(_signalWindow.begin(),i);
            break;
        }
    }
}

void NodDetector::read() {
    _GyroReading newReading;
    newReading.time = GetTickCount();
    EE_HeadsetGetGyroDelta(0,&newReading.x,&newReading.y);
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
        int minVal = std::numeric_limits<int>::max(),
            maxVal = std::numeric_limits<int>::min();
        DWORD minValTime,maxValTime;
        for(std::list<_GyroReading>::iterator i=_signalWindow.begin();
             i != _signalWindow.end();++i) {
            if(i->x < minVal) {
                minVal = i->x;
                minValTime = i->time;
            }
            if(i->x > maxVal) {
                maxVal = i->x;
                maxValTime = i->time;
            }
        }
        if(maxVal-minVal > MIN_INTENSITY && maxValTime != minValTime) {
            x = sign(maxVal-minVal)/sign(maxValTime-minValTime);
            _lastXPulse = std::max(maxValTime,minValTime);
        } else {
            x = 0;
        }
    } else {
        x = 0;
    }
    if(_lastYPulse < first.time && std::abs(first.y) <= MAX_BASELINE) {
        int minVal = std::numeric_limits<int>::max(),
            maxVal = std::numeric_limits<int>::min();
        DWORD minValTime,maxValTime;
        for(std::list<_GyroReading>::iterator i=_signalWindow.begin();
             i != _signalWindow.end();++i) {
            if(i->y < minVal) {
                minVal = i->y;
                minValTime = i->time;
            }
            if(i->y > maxVal) {
                maxVal = i->y;
                maxValTime = i->time;
            }
        }
        if(maxVal-minVal > MIN_INTENSITY && maxValTime != minValTime) {
            y = sign(maxVal-minVal)/sign(maxValTime-minValTime);
            _lastYPulse = std::max(maxValTime,minValTime);
        } else {
            y = 0;
        }
    } else {
        y = 0;
    }
}
