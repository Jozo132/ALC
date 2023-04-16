#pragma once
#include "../config.h"

#define _SEGMENT_NAME_ "[Segment 2-5 proga]"


struct segment_2_5_t: _vovk_plc_block_t {
    bool deska_vhodna_pripravljena = true;
    bool deska_izhodna_pripravljena = true;
    bool stanje_vmesne_deske = true;

    bool ima_vmesno_desko = true;

    void izhodisce() {
        // if (DEBUG_FLOW && running) Serial.printf(_SEGMENT_NAME_ " Konec\n");
        running = false;
        finished = false;
        M2_4 = false;
        M2_5 = false;
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
        FAZA_3_USTAVITEV
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
        deska_vhodna_pripravljena = P3;
        deska_izhodna_pripravljena = P5; // TODO: P4 za IZMET

        bool deska_vmesna_odmaknjena = !S2_12 && stanje_vmesne_deske;
        stanje_vmesne_deske = S2_12;

        bool deska_prisotna = S2_1;

        bool on = AUTO || ROCNO;
        bool work = running && on;
        if (work) {
            if (M2_4 && deska_vmesna_odmaknjena && deska_vhodna_pripravljena) {
                deska_vhodna_pripravljena = false;
                ima_vmesno_desko = true;
            }

            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    timer.set(100);
                    flow.next();
                    ima_vmesno_desko = false;
                    break;
                }
                case FAZA_1_ZAGON: {
                    if (timer.finished() && deska_vhodna_pripravljena && !deska_izhodna_pripravljena && IzmetacDobri.jeZadaj()) {
                        M2_4 = true;
                        M2_5 = false;
                        timer.set(500);
                        flow.next();
                    }
                    break;
                }
                case FAZA_2_ZAKASNJEN_ZAGON: {
                    if (timer.finished()) {
                        M2_4 = true;
                        M2_5 = true;
                        timer.set(25000); // timer za izklop proge
                        flow.next();
                    }
                    break;
                }
                case FAZA_3_USTAVITEV: {
                    if (timer.finished() || S2_9) {
                        cycle_counter++;
                        M2_4 = false;
                        M2_5 = false;
                        if (!ima_vmesno_desko) deska_vhodna_pripravljena = false; // Samo, ce ni deske na vmesnem senzorju
                        if (S2_9) deska_izhodna_pripravljena = true;
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
        P3 = deska_vhodna_pripravljena;
        P5 = deska_izhodna_pripravljena;
    }
};

segment_2_5_t segment_2_5;