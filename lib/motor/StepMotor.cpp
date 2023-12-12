//
// Created by gonzalezext on 12.12.23.
//

#include "StepMotor.h"

StepMotor::StepMotor() {
    pinMode(D0, OUTPUT);
    pinMode(D7, INPUT);
    for (int pin: pins) {
        pinMode(pin, OUTPUT);
    }

    state = 1;
    rotateFlag = false;
    sensorReading = false;
    rotateInt = new IntervalCheck(10);
}

void StepMotor::loop() {
    if (rotateFlag) {
        rotate();
    }
    readSensor();
}

void StepMotor::startRotation() {
    sensorReading = true;
}

void StepMotor::readSensor() {
    bool reading = digitalRead(D7) == LOW;
    if (reading && reading != sensorReading) {
        rotateFlag = !rotateFlag;
        if (!rotateFlag) {
            for (int pin: pins) {
                digitalWrite(pin, LOW);
            }
        }
    }
    sensorReading = reading;
}

void StepMotor::rotate() {
    if (rotateInt->canRun()) {
        for (int i = 0; i < 4; ++i) {
            digitalWrite(pins[i], (state & (1 << i)) > 0 ? HIGH : LOW);
        }

        state = (((state << 1) & 16) >> 4) | ((state << 1) & 15);
    }
}
