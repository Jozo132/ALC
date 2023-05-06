#pragma once
#include "../config.h"

#define _SEGMENT_NAME_ "[Segment 2-3 fliper]"


struct segment_2_3_t : _vovk_plc_block_t {
    bool deska_izhodna_prisotna = true;

    void izhodisce() {
        // if (DEBUG_FLOW && running) Serial.printf(_SEGMENT_NAME_ " Konec\n");
        running = false;
        finished = false;
        safe = true;
        Fliper.moveTo(0);
        blokada_z_strani_filperja = false;
        segment_2_3_prosto_za_desko = false;
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
        FAZA_0_ZAGON = 0,
        FAZA_1_FLIPER_1_GOR,
        FAZA_2_PRICAKAJ_PRIHOD_DESKE,
        FAZA_3_FLIPER_2_GOR,
        FAZA_4_FLIPER_1_IN_2_DOL
    };

    void loop() {
        if (!enabled && running) {  // ignore disabled blocks
            izhodisce();
            finished = true;
            safe = true;
        }
        deska_izhodna_prisotna = zalogovnik.prisotnaPrva();
        if (!enabled) return;

        int fliper_pozicija = Fliper.getPosition();

        bool fliper_spodaj = fliper_pozicija == 0;
        bool fliper_vmes = fliper_pozicija == 1;
        bool fliper_zgoraj = fliper_pozicija == 2;

        bool veriga_stoji = !M2_3;

        bool safe_to_start = !P2 && !deska_izhodna_prisotna && fliper_spodaj;


        if (P_5s) {
            Serial.printf(_SEGMENT_NAME_ " status: POZ: %d , VH: %c , IZH: %c , VS: %c , B: %c\n",
                fliper_pozicija,
                flipper_ima_desko ? 'Y' : 'N',
                deska_izhodna_prisotna ? 'Y' : 'N',
                veriga_stoji ? 'Y' : 'N',
                blokada_z_strani_filperja ? 'Y' : 'N'
            );
        }

        bool on = AUTO || ROCNO;
        bool work = running && on;
        if (work) {
            switch (flow.phase) {
                case FAZA_0_ZAGON: {
                    if (flipper_ima_desko || safe_to_start) flow.next();
                    break;
                }
                case FAZA_1_FLIPER_1_GOR: {
                    Fliper.moveTo(1);
                    if (fliper_vmes && !blokada_z_strani_filperja) {
                        if (AUTO) blokada_z_strani_filperja = true;
                        if (!flipper_ima_desko) segment_2_3_prosto_za_desko = true;
                        flow.next();
                    }
                    break;
                }
                case FAZA_2_PRICAKAJ_PRIHOD_DESKE: {
                    if (flipper_ima_desko && veriga_stoji && (AUTO || !blokada_z_strani_filperja)) {
                        blokada_z_strani_filperja = true;
                        segment_2_3_prosto_za_desko = false;
                        flow.next();
                    }
                    break;
                }
                case FAZA_3_FLIPER_2_GOR: {
                    Fliper.moveTo(2);
                    if (fliper_zgoraj) flow.next();
                    break;
                }
                case FAZA_4_FLIPER_1_IN_2_DOL: {
                    Fliper.moveTo(0);
                    if (fliper_spodaj) {
                        blokada_z_strani_filperja = false;
                        flipper_ima_desko = false;
                        zalogovnik.push();
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
    }
};

segment_2_3_t segment_2_3;