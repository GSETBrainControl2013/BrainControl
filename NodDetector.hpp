#ifndef NODDETECTOR_HPP
#define NODDETECTOR_HPP

#include <windows.h>
#include <list>

class NodDetector {
    static const DWORD WINDOW_DURATION = 1*1000;
    static const int MAX_BASELINE  = 3,
                     MIN_INTENSITY = 100;

    struct _GyroReading {
        int x,y;
        DWORD time;
    };
    std::list<_GyroReading> _signalWindow;
    DWORD _lastXPulse,_lastYPulse;

    void _fixWindow();
public:
    NodDetector() : _lastXPulse(0),_lastYPulse(0) {}
    void read();
    void getNod(int& x,int& y);
};

#endif
