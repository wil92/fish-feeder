#include <Arduino.h>

#define D0 16
#define D1 5
#define D2 4
#define D5 14
#define D6 12
#define D7 13
#define D8 15

// structure declaration
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

// variables declaration
int state = 1;
int pins[] = {D1, D2, D5, D6};
bool rotateFlag = false;
bool sensorReading = false;
IntervalCheck *rotateInt;

// function declaration
void rotate();
void readSensor();

void setup() {
    Serial.begin(9600);
    pinMode(D0, OUTPUT);
    pinMode(D7, INPUT);
    for (int i = 0; i < 4; ++i) {
        pinMode(pins[i], OUTPUT);
    }

    rotateInt = new IntervalCheck(5);
}

void loop() {
    if (rotateFlag) {
        rotate();
    }
    readSensor();
}

void readSensor() {
    bool reading = digitalRead(D7) == LOW;
    if (reading && reading != sensorReading) {
        rotateFlag = !rotateFlag;
        if (!rotateFlag) {
            for (int i = 0; i < 4; ++i) {
                digitalWrite(pins[i], LOW);
            }
        }
    }
    sensorReading = reading;
}

void rotate() {
    if (rotateInt->canRun()) {
        for (int i = 0; i < 4; ++i) {
            digitalWrite(pins[i], (state & (1 << i)) > 0 ? HIGH : LOW);
        }

        state = (((state << 1) & 16) >> 4) | ((state << 1) & 15);
    }
}
