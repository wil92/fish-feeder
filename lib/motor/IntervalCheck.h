//
// Created by gonzalezext on 12.12.23.
//

#ifndef FEEDER_INTERVALCHECK_H
#define FEEDER_INTERVALCHECK_H

#include <Arduino.h>

struct IntervalCheck {
    int lastTime;
    int interval;

    IntervalCheck(int interval) {
        lastTime = millis();
        this->interval = interval;
    }

    bool canRun() {
        int time = millis();
        bool res = time - lastTime >= interval;
        if (res) {
            lastTime = time;
        }
        return res;
    }
};

#endif //FEEDER_INTERVALCHECK_H
