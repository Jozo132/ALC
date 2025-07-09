#pragma once
// JavaScript setTimeout alternative
#include <Arduino.h>

#ifndef XTP_14A6_E

class Timeout {
private:
    bool enabled = true;
    bool triggered = true;
    long last_trigger = 0;
    long duration = 1000;
    long t = 0;
public:
    Timeout(long timeout = 1000) {
        this->duration = timeout;
    }
    bool check() {
        if (enabled && !triggered) {
            t = millis();
            if (t < this->last_trigger || t >= this->last_trigger + this->duration) {
                triggered = true;
                return true;
            }
        }
        return false;
    }
    bool finished() {
        this->check();
        return triggered;
    }
    void reset() {
        this->last_trigger = millis();
        triggered = false;
    }
    void clear() {
        enabled = false;
    }
    void set() {
        enabled = true;
        this->reset();
    }
    void set(long timeout) {
        this->duration = timeout;
        enabled = true;
        this->reset();
    }
};

#endif

class TOnDelay {
private:
    bool triggered = false;
    bool state = false;
    long last_trigger = 0;
    long duration = 1000;
    long t = 0;
public:
    TOnDelay(long timeout) {
        this->duration = timeout;
    }
    bool check(bool bit) {
        if (bit) {
            if (triggered) return true;
            if (!state) {
                state = true;
                last_trigger = millis();
            }
            t = millis();
            if (t < this->last_trigger || t >= this->last_trigger + this->duration) {
                triggered = true;
                return true;
            }
        } else {
            state = false;
            triggered = false;
        }
        return false;
    }
};