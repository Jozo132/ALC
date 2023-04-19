#pragma once
#include "../config.h"

#define _SEGMENT_NAME_ "[Segment 2-2 veriga]"

struct segment_2_2_t: _vovk_plc_block_t {
    bool deska_vhodna_prisotna = true;
    bool deska_izhodna_prisotna = true;

    void izhodisce() {
        // if (DEBUG_FLOW && running) Serial.printf(_SEGMENT_NAME_ " Konec\n");
        running = false;
        finished = false;
        safe = true;
        prva_veriga_varna = true;
        M2_2 = false;
        flow.reset();
    }

    void setup(bool state = 0) {
        enabled = state;
        flow.name = _SEGMENT_NAME_;
        flow.onChangePrint = DEBUG_FLOW_CHANGE;
        izhodisce();
    }

    Timeout t_zakasnitev;
    Timeout t_timeout;

    void run() {
        if (!running) {
            // if (DEBUG_FLOW) Serial.printf(_SEGMENT_NAME_ " Zagon\n");
            running = true;
            finished = false;
            flow.reset();
        }
    }
    void stop_soft() {
        if (running) {
            stopping = true;
        }
    }
    void stop_hard() {
        if (running) {
            izhodisce();
        }
    }

    enum {
        FAZA_0_PRICAKAJ_POGOJE = 0,
        FAZA_1_M2_ON,
        FAZA_2_AWAIT_S2_OFF,
        FAZA_3_AWAIT_S2_ON,
        FAZA_4_CHECK,
        FAZA_5_M2_OFF_AWAIT_DA_PADE,
    };

    void loop() {
        if (!enabled && running) {  // ignore disabled blocks
            izhodisce();
            finished = true;
            safe = true;
            prva_veriga_varna = true;
        }
        if (!enabled) return;

        deska_vhodna_prisotna = P1;
        deska_izhodna_prisotna = P2;

        bool on = AUTO || ROCNO || stopping;
        bool work = running && on;
        bool push_is_safe = deska_vhodna_prisotna && !deska_izhodna_prisotna && segment_2_3_prosto_za_desko && !flipper_ima_desko;
        if (work) {
            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    if (push_is_safe) {
                        Serial.printf(_SEGMENT_NAME_ " Pomik\n");
                        safe = false;
                        prva_veriga_varna = false;
                        finished = false;
                        flow.next();
                    }
                    break;
                }
                case FAZA_1_M2_ON: {
                    M2_2 = true;
                    if (!S2_2) flow.goTo(FAZA_3_AWAIT_S2_ON);
                    else flow.next();
                    break;
                }
                case FAZA_2_AWAIT_S2_OFF: {
                    if (flow.phaseSetup()) {
                        t_zakasnitev.set(500);
                        t_timeout.set(2000);
                    }
                    if (t_zakasnitev.finished()) {
                        if (!S2_2) {
                            errors_count[flow.phase] = 0;
                            flow.next();
                        } else if (t_timeout.finished()) {
                            errors_count[flow.phase]++;
                            flow.next();
                        }
                    }
                    break;
                }
                case FAZA_3_AWAIT_S2_ON: {
                    if (flow.phaseSetup()) {
                        t_zakasnitev.set(500);
                        t_timeout.set(2000);
                    }
                    if (t_zakasnitev.finished()) {
                        if (S2_2) {
                            errors_count[flow.phase] = 0;
                            flow.next();
                        } else if (t_timeout.finished()) {
                            errors_count[flow.phase]++;
                            flow.next();
                        }
                    }
                    break;
                }
                case FAZA_4_CHECK: {
                    deska_vhodna_prisotna = false;
                    deska_izhodna_prisotna = true;
                    flow.next();
                    break;
                }
                case FAZA_5_M2_OFF_AWAIT_DA_PADE: {
                    M2_2 = false;
                    flipper_ima_desko = true;
                    segment_2_3_prosto_za_desko = false;
                    if (flow.phaseSetup()) timer.set(500);
                    if (timer.finished()) {
                        safe = true;
                        prva_veriga_varna = true;
                        finished = true;
                        flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
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
        veriga_2_2_obratuje = M2_2;
        P1 = deska_vhodna_prisotna;
        P2 = deska_izhodna_prisotna;
    }
};

segment_2_2_t segment_2_2;