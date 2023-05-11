#pragma once
#include "../config.h"

#define _SEGMENT_NAME_ "[Segment 2-1 proga]"

struct segment_2_1_t : _vovk_plc_block_t {
    bool masina_lahko_dela = false;
    bool deska_izhodna_pripravljena = true;

    Timeout zakasnitev_izklopa;
    bool zakasnitev_aktivna = false;

    void izhodisce() {
        // if (running) Serial.printf(_SEGMENT_NAME_ " Konec\n");
        running = false;
        finished = false;
        safe = true;
        M2_1 = false;
        OUT_HOLD = true;
        masina_lahko_dela = false;
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
            //  Serial.printf(_SEGMENT_NAME_ " Zagon\n");
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
        FAZA_2_IZMET,
        FAZA_4_STATUS_UPDATE,
    };

    void loop() {
        if (!enabled && running) {  // ignore disabled blocks
            izhodisce();
            finished = true;
            safe = true;
        }
        if (!enabled) return;
        bool on = AUTO;
        bool work = running && on;

        deska_izhodna_pripravljena = P1;


        if (work) {
            bool deska_prisotna = S2_1;

            if (M2_1 && deska_prisotna) {
                zakasnitev_aktivna = true;
                zakasnitev_izklopa.set(1000);
            }

            M2_1 = flow.phase != FAZA_2_IZMET /* && !deska_izhodna_pripravljena */ && !deska_prisotna && !veriga_2_2_obratuje;

            if (M2_1) masina_lahko_dela = true;
            else {
                if (zakasnitev_aktivna && zakasnitev_izklopa.finished()) {
                    masina_lahko_dela = false;
                    zakasnitev_aktivna = false;
                }
            }

            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    if (deska_prisotna) flow.next();
                    break;
                }
                case FAZA_1_ZAGON: {
                    if (deska_prisotna) {
                        if (!deska_izhodna_pripravljena) {
                            deska_izhodna_pripravljena = true;
                            flow.next();
                        }
                    } else {
                        flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
                    }
                    break;
                }
                case FAZA_2_IZMET: {
                    // if (!deska_izhodna_pripravljena) flow.reset();
                    if (prva_veriga_varna && !veriga_2_2_obratuje) flow.reset();
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

        P1 = deska_izhodna_pripravljena; // P1 je prisotna, ce je proga deska_izhodna_pripravljena
        OUT_HOLD = !masina_lahko_dela; // Stroj lahko obdeluje samo kadar je proga aktivna
        safe = !M2_1; // Proga je varna, ce je izklopljena
    }
};

segment_2_1_t segment_2_1;