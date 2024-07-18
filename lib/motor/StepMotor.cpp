//
// Created by gonzalezext on 12.12.23.
//

#include "StepMotor.h"

StepMotor::StepMotor() {
    pinMode(D0, OUTPUT);
    pinMode(MAGNETIC_SENSOR_PIN, INPUT);
    for (int pin: pins) {
        pinMode(pin, OUTPUT);
    }

    sensorReading = false;
    rotateInt = new IntervalCheck(10);
}

void StepMotor::loop() {
    readMagneticSensor();
    if (isRunning()) {
        rotate();
    }
}

void StepMotor::startRotation() {
    if (!isRunning()) {
        state = START_ROTATING;
    }
}

void StepMotor::readMagneticSensor() {
    sensorReading = digitalRead(MAGNETIC_SENSOR_PIN) == LOW;
    if (!sensorReading && state == START_ROTATING) {
        state = ROTATING;
    }
    if (sensorReading && state == ROTATING) {
        state = STOPPED;
        sendStatusEvent();
    }
}

void StepMotor::rotate() {
    if (rotateInt->canRun()) {
        for (int i = 0; i < 4; ++i) {
            digitalWrite(pins[i], (motorState & (1 << i)) > 0 ? HIGH : LOW);
        }

        motorState = (((motorState << 1) & 16) >> 4) | ((motorState << 1) & 15);
    }
}

bool StepMotor::isRunning() {
    return state == START_ROTATING || state == ROTATING;
}

void StepMotor::setSendStatusEvent(SendStatusEvent sendStatusEventMethod) {
    this->sendStatusEvent = std::move(sendStatusEventMethod);
}
