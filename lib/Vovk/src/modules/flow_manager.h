#pragma once

class FlowManager {
public:
    bool initialized = false;
    int phase = 0;
    char* name = nullptr;
    bool onChangePrint = false;
    FlowManager() {}
    void reset() {
        initialized = false;
        if (phase == 0) return;
        if (onChangePrint && name != nullptr) {
            Serial.print("Flow reset @");
            Serial.println(name);
        }
        phase = 0;
        initialized = false;
    }
    void next() {
        phase++;
        initialized = false;
        if (onChangePrint && name != nullptr) {
            Serial.print("Flow next to ");
            Serial.print(phase);
            Serial.print(" @");
            Serial.println(name);
        }
    }
    void goTo(int p) {
        phase = p;
        initialized = false;
        if (onChangePrint && name != nullptr) {
            Serial.print("Flow jump to ");
            Serial.print(phase);
            Serial.print(" @");
            Serial.println(name);
        }
    }
    bool phaseSetup() {
        if (initialized) return false;
        initialized = true;
        return true;
    }
};