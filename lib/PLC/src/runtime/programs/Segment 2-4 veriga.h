#pragma once
#include "../config.h"

#define _SEGMENT_NAME_ "[Segment 2-4 veriga]"


struct segment_2_4_t: _vovk_plc_block_t {
    bool deska_vhodna_prisotna = true;
    bool deska_izhodna_prisotna = true;

    bool blokada_z_strani_filperja = false;

    Zalogovnik_t zalogovnik;
    int velikost_zalogovnika = 4;

    void izhodisce() {
        // if (DEBUG_FLOW && running) Serial.printf(_SEGMENT_NAME_ " Konec\n");
        running = false;
        finished = false;
        safe = true;
        M2_3 = false;
        flow.reset();
    }

    void setup(bool state = 0) {
        enabled = state;
        flow.name = _SEGMENT_NAME_;
        flow.onChangePrint = DEBUG_FLOW_CHANGE;
        izhodisce();
        zalogovnik.setup(velikost_zalogovnika);
        zalogovnik.pozicija[0] = 0;
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
        }
        if (!enabled) return;

        deska_vhodna_prisotna = P2;
        P3 = false;
        deska_izhodna_prisotna = P3;

        bool senzor_pohojen = S2_3;

        int stevilo_desk = zalogovnik.st_desk;

        bool on = AUTO || ROCNO || stopping;
        bool work = running && on;

        bool kombinacija_1_zagona = stevilo_desk > 0 && !deska_izhodna_prisotna;
        bool kombinacija_2_zagona = zalogovnik.prisotnaPrva() && !zalogovnik.prisotnaZadnja() && deska_izhodna_prisotna;

        bool push_is_safe = kombinacija_1_zagona || kombinacija_2_zagona;
        if (work) {
            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    M2_3 = false;
                    if (push_is_safe) {
                        if (kombinacija_1_zagona) Serial.printf(_SEGMENT_NAME_ " Pomik na valjcke\n");
                        if (kombinacija_2_zagona) Serial.printf(_SEGMENT_NAME_ " Vmesno polnjenje\n");
                        safe = false;
                        finished = false;
                        timer.set(100);
                        flow.next();
                    }
                    break;
                }
                case FAZA_1_M2_ON: {
                    if (timer.finished()) {
                        M2_3 = true;
                        if (!senzor_pohojen) flow.goTo(FAZA_3_AWAIT_S2_ON);
                        else flow.next();
                    }
                    break;
                }
                case FAZA_2_AWAIT_S2_OFF: {
                    if (flow.phaseSetup()) {
                        t_zakasnitev.set(100);
                        t_timeout.set(4000);
                    }
                    if (t_zakasnitev.finished()) {
                        if (!senzor_pohojen) {
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
                        t_zakasnitev.set(100);
                        t_timeout.set(4000);
                    }
                    if (t_zakasnitev.finished()) {
                        if (senzor_pohojen) {
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
                    bool single_move = deska_izhodna_prisotna && zalogovnik.safeToMoveOne();
                    bool prisotna_deska = zalogovnik.move();
                    deska_vhodna_prisotna = false;
                    if (prisotna_deska || SERVIS) {
                        // SERVIS_M2 = false;
                        deska_izhodna_prisotna = true;
                        // if (P2) cilindri.run();
                        flow.next();
                    } else {
                        if (single_move) {
                            M2_3 = false;
                            safe = true;
                            finished = true;
                        }
                        flow.goTo(FAZA_0_PRICAKAJ_POGOJE); // Podaj naslednjo desko
                    }
                    break;
                }
                case FAZA_5_M2_OFF_AWAIT_DA_PADE: {
                    M2_3 = false;
                    if (flow.phaseSetup()) timer.set(800);
                    if (timer.finished()) {
                        safe = true;
                        finished = true;
                        // if (AUTO)
                        flow.goTo(FAZA_0_PRICAKAJ_POGOJE);
                        // else izhodisce();
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
        P2 = deska_vhodna_prisotna;
        P3 = deska_izhodna_prisotna;
    }
};

segment_2_4_t segment_2_4;