#pragma once
#include <IWatchdog.h>
#include <Vovk.h>

bool& LED = AutoPins.attach(PC13, OUTPUT, INVERTED);

long cycleTime_us = 0;

// EXPANSION
#include <LSR32IO.h>

#define cs_pin PB9

#define sck_pin PB13
#define miso_pin PB14
#define mosi_pin PB15

#define latch_pin PB8
#define en_pin PB5
#define rst_pin PA15

LSR32IO expansion(cs_pin, latch_pin, en_pin, rst_pin);

#define DEBUG_FLOW_CHANGE true

#define DEBOUNCE_FAST 50 // Number of cycles to debounce inputs
#define DEBOUNCE_SLOW 1000 // Number of cycles to debounce inputs
#define DEBOUNCE_1s 5000 // Number of cycles to debounce inputs
#define DEBOUNCE_3s DEBOUNCE_1s * 3

// bool& SW_BTN_MCU = expansion.attachInputBit(31, DEBOUNCE_FAST);
// bool& SW_BTN_MCU = AutoPins.attach(PA0, INPUT_PULLUP);

// INPUTS
bool& EMERGENCY = expansion.attachInputBit(0, DEBOUNCE_FAST);                  // VHOD  0 - STATUS IZKLOP V SILI (invertiran)
bool& SW_A_START = expansion.attachInputBit(1, DEBOUNCE_FAST);                 // VHOD  1 - TIPKA START
bool& SW_A_STOP = expansion.attachInputBit(2, DEBOUNCE_FAST, INVERTED);        // VHOD  2 - TIPKA STOP (invertiran)
bool& SW_MODE_1 = expansion.attachInputBit(3, DEBOUNCE_FAST);                  // VHOD  3 - TIPKA MODE 1 (rezim delovanja)
bool& SW_DELOVANJE_IZMETA = expansion.attachInputBit(4, DEBOUNCE_FAST);                  // VHOD  4 - TIPKA MODE 2 (rezim delovanja)
bool& SW_IZHODISCE = expansion.attachInputBit(5, DEBOUNCE_FAST);               // VHOD  5 - TIPKA IZHODISCE
bool& VKLOP_SISTEMA = expansion.attachInputBit(6, DEBOUNCE_FAST);              // VHOD  6 - TIPKA VKLOP SISTEMA
bool& IZPAD_BIMETALA = expansion.attachInputBit(7, DEBOUNCE_FAST, INVERTED);   // VHOD  7 - IZPAD BIMETALA

bool& S2_1 = expansion.attachInputBit(8, DEBOUNCE_FAST, INVERTED);   // VHOD  8 - S2.1 PRISOTNOST LESA (KONEC)
bool& S2_2 = expansion.attachInputBit(9, DEBOUNCE_FAST);             // VHOD  9 - S2.2 POZICIJA KORAKA IZMETALNE VERIGE
bool& S2_3 = expansion.attachInputBit(10, DEBOUNCE_FAST);            // VHOD 10 - S2.3 POZICIJA KORAKA DVIZNE VERIGE
bool& S2_4 = expansion.attachInputBit(11, DEBOUNCE_FAST);            // VHOD 11 - S2.4 FLIPER SENZOR 1
bool& S2_5 = expansion.attachInputBit(12, DEBOUNCE_FAST);            // VHOD 12 - S2.5 FLIPER SENZOR 2
bool& S2_6 = expansion.attachInputBit(13, DEBOUNCE_FAST);            // VHOD 13 - S2.6 FLIPER SENZOR 3
bool& S2_7 = expansion.attachInputBit(14, DEBOUNCE_FAST);            // VHOD 14 - S2.7 FLIPER SENZOR 4
bool& S2_8 = expansion.attachInputBit(15, DEBOUNCE_SLOW);            // VHOD 15 - S2.8 POZICIJA DESKE VMESNA POZICIJA

bool& S2_9 = expansion.attachInputBit(16, DEBOUNCE_FAST, INVERTED);  // VHOD 16 - S2.9 POZICIJA LESA KONEC
bool& S2_12 = expansion.attachInputBit(17, DEBOUNCE_SLOW);           // VHOD 17 - S2.12 POZICIJA LESA NA VALJCNI 1
bool& S2_16 = expansion.attachInputBit(19, DEBOUNCE_SLOW);           // VHOD 18 - S2.16 POZICIJA LESA NA VALJCNI 2

bool& S2_10 = expansion.attachInputBit(20, DEBOUNCE_FAST);           // VHOD 20 - S2.10 IZMET SLABIH OD (OSNOVNI POLOZAJ)
bool& S2_11 = expansion.attachInputBit(21, DEBOUNCE_FAST);           // VHOD 21 - S2.11 IZMET SLABIH DP (DELOVNI POLOZAJ)
bool& S2_13 = expansion.attachInputBit(22, DEBOUNCE_FAST);           // VHOD 22 - S2.13 IZMET DOBRIH OD (OSNOVNI POLOZAJ)
bool& S2_14 = expansion.attachInputBit(23, DEBOUNCE_FAST);           // VHOD 23 - S2.14 IZMET DOBRIH DP (DELOVNI POLOZAJ)

bool& S2_15 = expansion.attachInputBit(24, DEBOUNCE_3s, INVERTED); // VHOD 24 - S2.15 PRISOTNOST KOSA PRED ZAGO (KONEC)

bool& ZAVESA = expansion.attachInputBit(25, DEBOUNCE_FAST);          // VHOD 25 - ZAVESA

bool& SCANNER_ON = expansion.attachInputBit(26, DEBOUNCE_FAST);     // VHOD 26 - SCANNER ENABLED
bool& SCANNER_BUSY = expansion.attachInputBit(27, DEBOUNCE_FAST);   // VHOD 27 - SCANNER BUSY

// OUTPUTS
bool& OUT_HOLD = expansion.attachOutputBit(0, INVERTED);  // IZHOD  0 - FREIGABE VKLOP
bool& ALARM = expansion.attachOutputBit(1);     // IZHOD  1 - ALARM

bool& B2_3 = expansion.attachOutputBit(2, INVERTED);  // IZHOD  2 - B2.3 BREMZA ZA MOTOR M2.3

bool& M2_1 = expansion.attachOutputBit(8);      // IZHOD  8 - M2.1 VALJCNA PROGA IZ SKOBELJNEGA
bool& M2_2 = expansion.attachOutputBit(9);      // IZHOD  9 - M2.2 IZMETALNA VERIGA
bool& M2_3 = expansion.attachOutputBit(10);     // IZHOD 10 - M2.3 DVIZNA VERIGA
bool& M2_4 = expansion.attachOutputBit(11);     // IZHOD 11 - M2.4 VALJCNA PROGA PRED SKENERJEM
bool& M2_5 = expansion.attachOutputBit(12);     // IZHOD 12 - M2.5 VALJCNA PROGA ZA SKENERJEM

bool& Y2_1A = expansion.attachOutputBit(16);    // IZHOD 16 - Y2.1A FLIPER 1 OD (OSNOVNI POLOZAJ)
bool& Y2_1B = expansion.attachOutputBit(17);    // IZHOD 17 - Y2.1B FLIPER 1 DP (DELOVNI POLOZAJ)
bool& Y2_2A = expansion.attachOutputBit(18);    // IZHOD 18 - Y2.2A FLIPER 2 OD (OSNOVNI POLOZAJ)
bool& Y2_2B = expansion.attachOutputBit(19);    // IZHOD 19 - Y2.2B FLIPER 2 DP (DELOVNI POLOZAJ)
bool& Y2_3A = expansion.attachOutputBit(20);    // IZHOD 20 - Y2.3A IZMET SLABIH OD (OSNOVNI POLOZAJ)
bool& Y2_3B = expansion.attachOutputBit(21);    // IZHOD 21 - Y2.3B IZMET SLABIH DP (DELOVNI POLOZAJ)
bool& Y2_4A = expansion.attachOutputBit(22);    // IZHOD 22 - Y2.4A IZMET DOBRIH OD (OSNOVNI POLOZAJ)
bool& Y2_4B = expansion.attachOutputBit(23);    // IZHOD 23 - Y2.4B IZMET DOBRIH DP (DELOVNI POLOZAJ)

// bool& S_TEST = expansion.attachInputBit(30, DEBOUNCE_3s);   // VHOD  30 - TEST INPUT S_TEST
// bool& Y1_TEST = expansion.attachOutputBit(29);               // IZHOD 30 - TEST OUTPUT Y1_TEST
// bool& Y2_TEST = expansion.attachOutputBit(30);               // IZHOD 32 - TEST OUTPUT Y2_TEST

struct Fliper_t {
    bool state = false;
    Timeout timeout = Timeout(100);
    int current_position = 0;
    void _set_state(int position) {
        if (position == 0) {
            Y2_1A = true;
            Y2_1B = false;
            Y2_2A = true;
            Y2_2B = false;
            return;
        }
        if (position == 1) {
            Y2_1A = false;
            Y2_1B = true;
            Y2_2A = true;
            Y2_2B = false;
            return;
        }
        if (position == 2) {
            Y2_1A = false;
            Y2_1B = true;
            Y2_2A = false;
            Y2_2B = true;
            return;
        }
        Y2_1A = false;
        Y2_1B = false;
        Y2_2A = false;
        Y2_2B = false;
    }
    void moveTo(int position = 0) {
        current_position = position;
    }
    int getPosition() {
        if (!S2_4 && S2_5 && !S2_6 && S2_7) return 0;
        if (S2_4 && !S2_5 && !S2_6 && S2_7) return 1;
        if (S2_4 && !S2_5 && S2_6 && !S2_7) return 2;
        return -1;
    }
    void sync() {
        _set_state(current_position);
        // if (state) {
        //     if (timeout.finished()) {
        //         _set_state(current_position);
        //         state = !state;
        //         timeout.set(200);
        //     }
        //     return;
        // }
        // if (timeout.finished()) {
        //     _set_state(-1);
        //     state = !state;
        //     timeout.set(1800);
        // }

    }
} Fliper;

struct IzmetacDobri_t {
    bool jeZdajState = true;
    int direction = -1;
    void naprej() {
        Y2_4A = false;
        Y2_4B = true;
        direction = 1;
        jeZdajState = false;
    }
    void nazaj() {
        Y2_4A = true;
        Y2_4B = false;
        direction = -1;
    }
    bool jeSpredaj() {
        return !S2_13 && S2_14;
    }
    bool jeZadaj() {
        bool zadajPulseUP = S2_13 && !S2_14;
        if (direction == -1 && zadajPulseUP) {
            jeZdajState = true;
        }
        return jeZdajState;
    }
} IzmetacDobri;

bool P1 = false;  // Deska prisotna na koncu traka
bool P2 = true;   // Deska prisotna na odlagalnem mestu
bool P3 = true;   // Deska na zgornjem traku
bool P4 = true;   // Deska na vmesni poziciji
bool P5 = true;  // Deska na koncu traka
bool P6 = true;  // Deska na vhodu zage

bool prva_veriga_varna = false;
bool veriga_2_2_obratuje = false;
bool segment_2_3_prosto_za_desko = false;
bool flipper_ima_desko = false;
bool blokada_z_strani_filperja = false;
bool zgornja_proga_obratuje = false;


TOnDelay ton_deskaZgoraj(2000);
TOnDelay tof_deskaZgoraj(2000);
bool deskaZgoraj_out = false;
bool deskaZgoraj() {
    bool pogoj = S2_12 || S2_16;

    // Y1_TEST = ton_deskaZgoraj.check(S_TEST);
    // Y2_TEST = tof_deskaZgoraj.check(!S_TEST);

    // if (ton_zgoraj) deskaZgoraj_out = true;
    // if (tof_zgoraj) deskaZgoraj_out = false;
    // return deskaZgoraj_out;
    return pogoj;
}

bool AUTO = false;   // Mode AUTO
bool ROCNO = false;  // Mode ROCNO
bool SERVIS = false; // Mode SERVIS
bool STOP = true;


int cycle_counter = 0;

#define DVOJNI_ZAGON  true


double last_idle_time = 0;
double idle_sec = 0;

double spool_hold_time = 1.0;

#define max_st_desk 10

struct Zalogovnik_t {
    uint32_t velikost_zalogovnika = 0;
    int st_desk = max_st_desk;
    bool pozicija[max_st_desk] = { 1 };
    void setup(int size = max_st_desk) {
        velikost_zalogovnika = size;
        if (velikost_zalogovnika > max_st_desk) velikost_zalogovnika = max_st_desk;
        for (int i = 0; i < velikost_zalogovnika; i++) pozicija[i] = 1;
        st_desk = velikost_zalogovnika;
    }
    void push() {
        if (pozicija[0] == 0) {
            pozicija[0] = 1;
            st_desk++;
        }
    }
    bool move() {
        bool output = pozicija[velikost_zalogovnika - 1];
        st_desk = 0;
        for (int i = velikost_zalogovnika - 1; i > 0; i--) {
            pozicija[i] = pozicija[i - 1];
            if (pozicija[i]) st_desk++;
        }
        pozicija[0] = 0;
        return output;
    }
    void empty() {
        for (int i = 0; i < velikost_zalogovnika; i++) pozicija[i] = 0;
        st_desk = 0;
    }
    bool prisotnaPrva() {
        return pozicija[0] == 1;
    }
    bool prisotnaZadnja() {
        return pozicija[velikost_zalogovnika - 1] == 1;
    }
    bool safeToMoveOne() {
        return pozicija[0] == 1 && pozicija[velikost_zalogovnika - 1] == 0;
    }
    int getStDesk() {
        return st_desk;
    }
};



Zalogovnik_t zalogovnik;
int velikost_zalogovnika = 4;
