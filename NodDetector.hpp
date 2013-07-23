#ifndef NODDETECTOR_HPP
#define NODDETECTOR_HPP

#include <windows.h>
#include <list>
#include <iostream>

class NodDetector {
    static const DWORD WINDOW_DURATION = 1*1000;
    static const int MAX_BASELINE  = 3,
                     MIN_INTENSITY = 130;

    struct _GyroReading {
        int x,y;
        DWORD time;
    };
    std::list<_GyroReading> _signalWindow;
    DWORD _lastXPulse,_lastYPulse;
    DWORD _lastLog;

    void _fixWindow();
public:
    NodDetector();
    void read(std::ostream& log);
    void getNod(int& x,int& y);
};

#endif
