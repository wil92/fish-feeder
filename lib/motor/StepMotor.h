//
// Created by gonzalezext on 12.12.23.
//

#ifndef FEEDER_STEPMOTOR_H
#define FEEDER_STEPMOTOR_H

#include <memory>

#include "IntervalCheck.h"

#define D0 16
#define D1 5
#define D2 4
#define D5 14
#define D6 12
#define D7 13
#define D8 15

#define MAGNETIC_SENSOR_PIN D7

enum StepMotorState {
    STOPPED,
    START_ROTATING,
    ROTATING
};

typedef std::function<void()> SendStatusEvent;

class StepMotor {
private:
    IntervalCheck *rotateInt;

    // true if the sensor is reading a magnetic field
    bool sensorReading = false;
    StepMotorState state = ROTATING;

    int pins[4] = {D1, D2, D5, D6};
    int motorState = 0;

    SendStatusEvent sendStatusEvent;

    void rotate();

    void writeMotorState();

    void readMagneticSensor();

public:
    StepMotor();

    void loop();

    bool isRunning();

    void startRotation();

    void stopRotation();

    void setSendStatusEvent(SendStatusEvent sendStatusEventMethod);
};

#endif //FEEDER_STEPMOTOR_H
