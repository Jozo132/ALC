#pragma once
#include "../config.h"

#define _SEGMENT_NAME_ "[Segment 2-4 veriga]"


struct segment_2_4_t : _vovk_plc_block_t {
    bool deska_vhodna_prisotna = true;
    bool deska_izhodna_prisotna = true;

    bool force_single_move = true;

    void izhodisce() {
        // if (DEBUG_FLOW && running) Serial.printf(_SEGMENT_NAME_ " Konec\n");
        running = false;
        finished = false;
        blokada_z_strani_filperja = false;
        safe = true;
        motor_stop();
        flow.reset();
    }

    void setup(bool state = 0) {
        enabled = state;
        flow.name = _SEGMENT_NAME_;
        flow.onChangePrint = DEBUG_FLOW_CHANGE;
        izhodisce();
        zalogovnik.setup(velikost_zalogovnika);
        // zalogovnik.pozicija[0] = 1;
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

    void motor_stop() {
        M2_3 = false;
        B2_3 = true;
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

        bool prvaPrisotna = zalogovnik.prisotnaPrva();
        bool zadnjaPrisotna = zalogovnik.prisotnaZadnja();

        deska_vhodna_prisotna = prvaPrisotna;
        deska_izhodna_prisotna = P3;

        bool senzor_pohojen = S2_3;

        int stevilo_desk = zalogovnik.getStDesk();

        bool on = AUTO || ROCNO || stopping;
        bool work = running && on;

        bool kombinacija_1_zagona = ROCNO && stevilo_desk > 0 && !deska_izhodna_prisotna;
        bool kombinacija_2_zagona = AUTO && prvaPrisotna && !deska_izhodna_prisotna;
        bool kombinacija_3_zagona = prvaPrisotna && !zadnjaPrisotna && deska_izhodna_prisotna;

        if (P_5s) {
            Serial.printf(_SEGMENT_NAME_ " status: DESKE: %d , IZH: %c , PP: %c , ZP: %c , BLOK: %c, k1: %c, k2: %c, k3: %c, S2_12: %c, S2_16: %c\n",
                stevilo_desk,
                deska_izhodna_prisotna ? 'Y' : 'N',
                prvaPrisotna ? 'Y' : 'N',
                zadnjaPrisotna ? 'Y' : 'N',
                blokada_z_strani_filperja ? 'Y' : 'N',
                kombinacija_1_zagona ? 'Y' : 'N',
                kombinacija_2_zagona ? 'Y' : 'N',
                kombinacija_3_zagona ? 'Y' : 'N',
                S2_12 ? 'Y' : 'N',
                S2_16 ? 'Y' : 'N'
            );
        }

        bool push_is_safe = (ROCNO || !blokada_z_strani_filperja) && (kombinacija_1_zagona || kombinacija_2_zagona || kombinacija_3_zagona);
        if (work) {
            switch (flow.phase) {
                case FAZA_0_PRICAKAJ_POGOJE: {
                    motor_stop();
                    if (push_is_safe) {
                        blokada_z_strani_filperja = true;
                        if (kombinacija_1_zagona) Serial.printf(_SEGMENT_NAME_ " Pomik na valjcke rocno\n");
                        else if (kombinacija_2_zagona) Serial.printf(_SEGMENT_NAME_ " Pomik na valjcke avtomatsko\n");
                        else if (kombinacija_3_zagona) Serial.printf(_SEGMENT_NAME_ " Pomik na valjcke avtomatsko (izhodna deska prisotna)\n");
                        safe = false;
                        finished = false;
                        B2_3 = false; // Izklop bremze
                        timer.set(200);
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
                    if (prisotna_deska || flipper_ima_desko || SERVIS) {
                        flow.next();
                    } else {
                        blokada_z_strani_filperja = false;
                        if (single_move || force_single_move) {
                            motor_stop();
                            safe = true;
                            finished = true;
                            flow.reset();
                        } else {
                            timer.set(100);
                            flow.goTo(FAZA_1_M2_ON); // Podaj naslednjo desko
                        }
                    }
                    break;
                }
                case FAZA_5_M2_OFF_AWAIT_DA_PADE: {
                    motor_stop();
                    if (flow.phaseSetup()) timer.set(1000);
                    if (timer.finished()) {
                        safe = true;
                        finished = true;
                        flow.reset();
                        blokada_z_strani_filperja = false;
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
    }
};

segment_2_4_t segment_2_4;