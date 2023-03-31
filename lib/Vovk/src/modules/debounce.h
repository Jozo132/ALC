#pragma once

#ifndef __VOVK_DEBOUNCE
#define __VOVK_DEBOUNCE
class Debounce {
private:
    int count = 0;
    int count_target = 50;
    bool previous_state = false;
    bool debounced_state = false;

public:
    Debounce(bool initialState = false, int debounce_count = 1) {
        count_target = debounce_count > 0 ? debounce_count : count_target;
        debounced_state = initialState;
    }
    bool update(bool state) {
        if (state == previous_state && count < count_target)
            count++;
        if (state != previous_state)
            count = 0;
        if (count >= count_target)
            debounced_state = state;
        previous_state = state;
        return debounced_state;
    }
};
#endif // __VOVK_DEBOUNCE