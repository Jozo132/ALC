#pragma once
#include "../config.h"

#define _SEGMENT_NAME_ "[Segment 2-7 izmet dobri]"


struct segment_2_7_t: _vovk_plc_block_t {
    bool deska_vhodna_pripravljena = true;
    bool deska_izhodna_pripravljena = true;

    void izhodisce() {
        // if (DEBUG_FLOW && running) Serial.printf(_SEGMENT_NAME_ " Konec\n");
        running = false;
        finished = false;
        IzmetacDobri.nazaj();
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
        FAZA_1_NAPREJ,
        FAZA_2_NAZAJ
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
        P5 = S2_9;
        deska_vhodna_pripravljena = P5;
        deska_izhodna_pripravljena = false; // P6 
        // if (P_5s) {
        //     Serial.printf("deska_vhodna_pripravljena: %c   deska_izhodna_pripravljena: %c   IzmetacDobri.jeZadaj(): %c\n", deska_vhodna_pripravljena ? '1' : '0', deska_izhodna_pripravljena ? '1' : '0', IzmetacDobri.jeZadaj() ? '1' : '0');
        // }
        bool on = AUTO || ROCNO;
        bool work = running && on;
        if (work) {
            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    IzmetacDobri.nazaj();
                    flow.next();
                    break;
                }
                case FAZA_1_NAPREJ: {
                    if (deska_vhodna_pripravljena && !deska_izhodna_pripravljena && IzmetacDobri.jeZadaj()) {
                        if (S2_9) IzmetacDobri.naprej();
                        timer.set(5000);
                        flow.next();
                    }
                    break;
                }
                case FAZA_2_NAZAJ: {
                    if (IzmetacDobri.jeSpredaj() || timer.finished()) {
                        deska_vhodna_pripravljena = false;
                        deska_izhodna_pripravljena = true;
                        flow.reset();
                    }
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
        P5 = deska_vhodna_pripravljena;
    }
};

segment_2_7_t segment_2_7;