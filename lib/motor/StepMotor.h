//
// Created by gonzalezext on 12.12.23.
//

#ifndef FEEDER_STEPMOTOR_H
#define FEEDER_STEPMOTOR_H

#include "IntervalCheck.h"

#define D0 16
#define D1 5
#define D2 4
#define D5 14
#define D6 12
#define D7 13
#define D8 15

class StepMotor {
private:
    IntervalCheck *rotateInt;
    bool rotateFlag = false;
    bool sensorReading = false;
    int state;
    int pins[4] = {D1, D2, D5, D6};

    void rotate();

    void readSensor();

public:
    StepMotor();

    void loop();

    void startRotation();
};

#endif //FEEDER_STEPMOTOR_H
