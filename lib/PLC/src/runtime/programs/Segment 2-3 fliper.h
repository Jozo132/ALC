#pragma once
#include "../config.h"
#include "Segment 2-4 veriga.h"

#define _SEGMENT_NAME_ "[Segment 2-3 fliper]"


struct segment_2_3_t: _vovk_plc_block_t {
    bool deska_vhodna_prisotna = true;
    bool deska_izhodna_prisotna = true;

    bool prosto_za_desko = false;

    void izhodisce() {
        // if (DEBUG_FLOW && running) Serial.printf(_SEGMENT_NAME_ " Konec\n");
        running = false;
        finished = false;
        safe = true;
        Fliper.moveTo(0);
        segment_2_4.blokada_z_strani_filperja = false;
        prosto_za_desko = false;
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
        deska_vhodna_prisotna = P2;
        deska_izhodna_prisotna = segment_2_4.zalogovnik.prisotnaPrva();
        if (!enabled) return;

        int fliper_pozicija = Fliper.getPosition();

        bool fliper_spodaj = fliper_pozicija == 0;
        bool fliper_vmes = fliper_pozicija == 1;
        bool fliper_zgoraj = fliper_pozicija == 2;

        bool safe_to_start = !deska_vhodna_prisotna && !deska_izhodna_prisotna && fliper_spodaj;

        bool on = AUTO || ROCNO;
        bool work = running && on;
        if (work) {
            switch (flow.phase) {
                case FAZA_0_ZAGON: {
                    if (safe_to_start) flow.next();
                    break;
                }
                case FAZA_1_FLIPER_1_GOR: {
                    Fliper.moveTo(1);
                    if (fliper_vmes) {
                        prosto_za_desko = true;
                        flow.next();
                    }
                    break;
                }
                case FAZA_2_PRICAKAJ_PRIHOD_DESKE: {
                    bool veriga_stoji = !M2_3;
                    if (deska_vhodna_prisotna && veriga_stoji) {
                        segment_2_4.blokada_z_strani_filperja = true;
                        prosto_za_desko = false;
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
                        segment_2_4.blokada_z_strani_filperja = false;
                        segment_2_4.zalogovnik.push();
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