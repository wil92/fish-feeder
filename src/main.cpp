#include <Arduino.h>

#define D1 5
#define D2 4
#define D5 14
#define D6 12

void setup() {
    // write your initialization code here
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
}

void rotate();

void loop() {
    // write your code here
    rotate();
    delay(20);
}

int state = 1;
int pins[] = {D1, D2, D5, D6};

void rotate() {
    for (int i = 0; i < 4; ++i) {
        digitalWrite(pins[i], (state & (1 << i)) > 0 ? HIGH : LOW);
    }

    state = (((state << 1) & 16) >> 4) | ((state << 1) & 15);
}
