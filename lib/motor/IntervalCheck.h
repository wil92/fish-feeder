//
// Created by gonzalezext on 12.12.23.
//

#ifndef FEEDER_INTERVALCHECK_H
#define FEEDER_INTERVALCHECK_H

#include <Arduino.h>

#define ull unsigned long long

struct IntervalCheck {
    ull lastTime;
    ull interval;

    explicit IntervalCheck(const int interval) {
        lastTime = millis();
        this->interval = static_cast<unsigned long long>(interval);
    }

    bool canRun() {
        const ull time = millis();
        const bool res = time - lastTime >= interval;
        if (res) {
            lastTime = time;
        }
        return res;
    }
};

#endif //FEEDER_INTERVALCHECK_H
