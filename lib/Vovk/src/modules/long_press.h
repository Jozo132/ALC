#pragma once
#include <Arduino.h>

class LongPress {
private:
    bool output;
    uint32_t timeOfPress;
    uint32_t targetTime = 1000;
    bool state = false;
    bool previous_state = false;
public:
    LongPress(uint32_t duration_ms = 1000) {
        if (duration_ms > 0) targetTime = duration_ms;
    }
    bool check(bool state) {
        output = false;
        if (state) {
            if (!previous_state || millis() < timeOfPress) timeOfPress = millis();
            if (millis() >= timeOfPress + targetTime)
                output = true;
        }
        previous_state = state;
        return output;
    }
};