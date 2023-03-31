#pragma once
#define _VOVK_MAX_DIGITAL_IO_PINS 100

#include <Arduino.h>

struct __vovk_pin_manager_t {
    int used = 0;
    bool unused_pin;
    int pins[_VOVK_MAX_DIGITAL_IO_PINS] = { -1 };
    int modes[_VOVK_MAX_DIGITAL_IO_PINS] = { -1 };
    bool inverteds[_VOVK_MAX_DIGITAL_IO_PINS] = { false };
    bool states[_VOVK_MAX_DIGITAL_IO_PINS] = { false };

    bool& attach(int pin, int mode, bool inverted = false) {
        if (used >= _VOVK_MAX_DIGITAL_IO_PINS) return unused_pin; // Max pins used!
        for (int i = 0; i < used; i++)
            if (pins[i] == pin)
                return unused_pin; // Pin already exists!
        int p = used;
        pins[p] = pin;
        modes[p] = mode;
        inverteds[p] = inverted;
        used++;
        pinMode(pin, mode);
        return states[p];
    }

    void loop() {
        int pin, mode;
        bool inverted, state;
        for (int i = 0; i < used; i++) {
            pin = pins[i];
            mode = modes[i];
            inverted = inverteds[i];
            state = states[i];
            switch (mode) {
            case OUTPUT:
            case OUTPUT_OPEN_DRAIN:
                digitalWrite(pin, inverted ? !state : state);
                break;
            case INPUT:
            case INPUT_PULLUP:
            case INPUT_PULLDOWN:
                state = digitalRead(pin);
                states[i] = inverted ? !state : state;
            }
        }
    }
} AutoPins;