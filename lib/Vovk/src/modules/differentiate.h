#pragma once
#include <Arduino.h>

class DifferentiateBit {
private:
    bool* refBit;
    bool new_state = false;
    bool previous_state = false;
    int change_direction;
public:
    DifferentiateBit(bool& bit, int direction = RISING, bool initial_state = false) {
        *refBit = bit;
        previous_state = initial_state;
        if (direction == RISING || direction == FALLING || direction == CHANGE)
            change_direction = direction;
        else
            change_direction = RISING;
    }
    bool check() {
        new_state = *refBit;
        bool output = false;
        if (new_state && !previous_state && (change_direction == CHANGE || change_direction == RISING))
            output = true;
        if (!new_state && previous_state && (change_direction == CHANGE || change_direction == FALLING))
            output = true;
        previous_state = new_state;
        return output;
    }
};