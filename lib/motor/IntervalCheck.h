//
// Created by gonzalezext on 12.12.23.
//

#ifndef FEEDER_INTERVALCHECK_H
#define FEEDER_INTERVALCHECK_H

#include <Arduino.h>

#define ull unsigned long long

struct IntervalCheck {
    ull lastTime;
    int interval;

    explicit IntervalCheck(int interval) {
        lastTime = millis();
        this->interval = interval;
    }

    bool canRun() {
        ull time = millis();
        bool res = time - lastTime >= interval;
        if (res) {
            lastTime = time;
        }
        return res;
    }
};

#endif //FEEDER_INTERVALCHECK_H
