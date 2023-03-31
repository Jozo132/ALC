#pragma once
#include <Arduino.h>

class OffDelay {
private:
    bool* in;
    bool* out;
    bool was_on = false;
    bool state = false;
    long off_time = 0;
    long timeout = 0;
public:
    OffDelay(bool& input, bool& output, long ms = 1000) {
        *in = input;
        *out = output;
        timeout = ms;
    }
    bool check() {
        long t = millis();
        bool output = false;
        bool new_state = *in;
        if (new_state) {
            was_on = true;
            output = true;
        }
        if (was_on) {
            if (!new_state && state) off_time = t;
            if (t > (off_time + timeout)) output = true;
        }
        state = new_state;
        *out = output;
        return output;
    }
};