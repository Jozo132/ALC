#pragma once
#include "../config.h"

#define _SEGMENT_NAME_ "[Segment 2-5 proga]"


struct segment_2_5_t: _vovk_plc_block_t {
    bool deska_izhodna_pripravljena = true;

    void izhodisce() {
        // if (DEBUG_FLOW && running) Serial.printf(_SEGMENT_NAME_ " Konec\n");
        running = false;
        finished = false;
        safe = true;
        flow.reset();
    }

    void setup(bool state = 0) {
        enabled = state;
        flow.name = _SEGMENT_NAME_;
        flow.onChangePrint = DEBUG_FLOW_CHANGE;
        izhodisce();
    }

    void run() {
        if (!running) {
            // if (DEBUG_FLOW) Serial.printf(_SEGMENT_NAME_ " Zagon\n");
            safe = false;
            running = true;
            finished = false;
            flow.reset();
        }
    }
    void stop() {
        if (running) {
            izhodisce();
        }
    }

    enum {
        FAZA_0_PRICAKAJ_POGOJE = 0,
        FAZA_1_ZAGON,
        FAZA_2_AWAIT_P1_ON,
        FAZA_2_IZMET,
        FAZA_4_STATUS_UPDATE,
    };

    void loop() {
        if (!enabled && running) {  // ignore disabled blocks
            izhodisce();
            finished = true;
            safe = true;
        }
        if (!enabled) {
            return;
        }

        deska_izhodna_pripravljena = P3;

        bool deska_prisotna = S2_1;

        bool on = AUTO || ROCNO;
        bool work = running && on;
        if (work) {
            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    if (deska_prisotna) flow.goTo(FAZA_2_AWAIT_P1_ON);
                    else flow.next();
                    break;
                }
                case FAZA_1_ZAGON: {
                    if (segment_2_2.safe && !deska_izhodna_pripravljena) {
                        deska_izhodna_pripravljena = false;
                        // M2_1 = true;
                        flow.next();
                    }
                    break;
                }
                case FAZA_2_AWAIT_P1_ON: {
                    if (deska_prisotna) {
                        timer.set(3000); // timer za izklop proge
                        flow.next();
                    }
                    break;
                }
                case FAZA_2_IZMET: {
                    // if (timer.finished()) M2_1 = false; // Avtomatska ustavitev proge po 3s, ce deska ni izmetana
                    deska_izhodna_pripravljena = true;
                    if (!deska_prisotna) flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
                    break;
                }

                default: {
                    izhodisce();
                    break;
                }
            }
        } else {
            izhodisce();
        }
    }
};

segment_2_5_t segment_2_5;