#pragma once
#include "../config.h"

#define _SEGMENT_NAME_ "[Segment 2-5 proga]"

TOnDelay scanner_freigabe(2000);

struct segment_2_5_t : _vovk_plc_block_t {
    bool deska_vhodna_pripravljena = true;
    bool deska_izhodna_pripravljena = true;

    bool deska_na_izhodu = false;

    bool scanner_busy = false;
    bool scanner_on = false;
    bool scanner_allow = false;

    bool M2_4_marker = false;

    Timer timeout;

    void izhodisce() {
        // if (DEBUG_FLOW && running) Serial.printf(_SEGMENT_NAME_ " Konec\n");
        running = false;
        finished = false;
        M2_4 = false;
        M2_4_marker = false;
        M2_5 = false;
        zgornja_proga_obratuje = false;
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
        FAZA_2_ZAKASNJEN_ZAGON,
        FAZA_3_ZAZNAVANJE_ON,
        FAZA_4_ZAZNAVANJE_OFF,
        FAZA_5_USTAVITEV,
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

        P3 = deskaZgoraj(); // Deska prisotna na zacetku

        deska_vhodna_pripravljena = P3;
        // TODO: P4 za IZMET
        deska_izhodna_pripravljena = P5;

        scanner_on = SCANNER_ON;
        scanner_busy = SCANNER_BUSY;
        scanner_allow = scanner_on && !scanner_busy;

        bool scanner_block = scanner_freigabe.check(!scanner_allow);

        // bool deska_prisotna = S2_1;
        bool deska_vmes = S2_8;
        bool deska_prisotna = S2_9;

        bool on = AUTO || ROCNO;
        bool work = running && on;
        if (work) {

            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    timer.set(100);
                    flow.next();
                    break;
                }
                case FAZA_1_ZAGON: {
                    if (timer.finished() && (deska_vhodna_pripravljena || deska_vmes) && !deska_izhodna_pripravljena && IzmetacDobri.jeZadaj()) {
                        zgornja_proga_obratuje = true;
                        M2_4_marker = true;
                        M2_5 = false;
                        timer.set(50);
                        flow.next();
                        timeout.set(25000); // timer za izklop proge
                    }
                    break;
                }
                case FAZA_2_ZAKASNJEN_ZAGON: {
                    if (timer.finished()) {
                        zgornja_proga_obratuje = true;
                        M2_4_marker = true;
                        M2_5 = true;
                        flow.next();
                    } else if (timeout.finished()) izhodisce();
                    break;
                }
                case FAZA_3_ZAZNAVANJE_ON: {
                    if (deska_prisotna) {
                        deska_izhodna_pripravljena = true;
                        timer.set(20);
                        flow.next();
                    } else if (timeout.finished()) izhodisce();
                    break;
                }
                case FAZA_4_ZAZNAVANJE_OFF: {
                    if (timer.finished() && !deska_prisotna) {
                        flow.next();
                    } else if (timeout.finished()) izhodisce();
                    break;
                }
                case FAZA_5_USTAVITEV: {
                    cycle_counter++;
                    M2_4_marker = false;
                    M2_5 = false;
                    zgornja_proga_obratuje = false;
                    deska_na_izhodu = false;
                    flow.reset();
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
        P5 = deska_izhodna_pripravljena;

        // Prepre?i premik valj?kov ko skenira
        M2_4 = M2_4_marker && !scanner_block;
    }
};

segment_2_5_t segment_2_5;