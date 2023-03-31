#pragma once
#include <IWatchdog.h>
#include <Vovk.h>

bool& LED = AutoPins.attach(PC13, OUTPUT, INVERTED);

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

#define BUTTON_DEBOUNCE_CYCLES 30

bool& SW_BTN_MCU = expansion.attachInputBit(31, BUTTON_DEBOUNCE_CYCLES);

// INPUTS
bool& EMERGENCY = expansion.attachInputBit(0, BUTTON_DEBOUNCE_CYCLES);                  // VHOD  0 - STATUS IZKLOP V SILI (invertiran)
bool& SW_A_START = expansion.attachInputBit(1, BUTTON_DEBOUNCE_CYCLES);                 // VHOD  1 - TIPKA START
bool& SW_A_STOP = expansion.attachInputBit(2, BUTTON_DEBOUNCE_CYCLES, INVERTED);        // VHOD  2 - TIPKA STOP (invertiran)
bool& SW_MODE_1 = expansion.attachInputBit(3, BUTTON_DEBOUNCE_CYCLES);                  // VHOD  3 - TIPKA MODE 1 (režim delovanja)
bool& SW_MODE_2 = expansion.attachInputBit(4, BUTTON_DEBOUNCE_CYCLES);                  // VHOD  4 - TIPKA MODE 2 (režim delovanja)
bool& SW_IZHODISCE = expansion.attachInputBit(5, BUTTON_DEBOUNCE_CYCLES);               // VHOD  5 - TIPKA IZHODISCE
bool& VKLOP_SISTEMA = expansion.attachInputBit(6, BUTTON_DEBOUNCE_CYCLES);              // VHOD  6 - TIPKA VKLOP SISTEMA
bool& IZPAD_BIMETALA = expansion.attachInputBit(7, BUTTON_DEBOUNCE_CYCLES, INVERTED);   // VHOD  7 - IZPAD BIMETALA

bool& S2_1 = expansion.attachInputBit(8, BUTTON_DEBOUNCE_CYCLES, INVERTED);   // VHOD  8 - S2.1 PRISOTNOST LESA (KONEC)
bool& S2_2 = expansion.attachInputBit(9, BUTTON_DEBOUNCE_CYCLES);             // VHOD  9 - S2.2 POZICIJA KORAKA IZMETALNE VERIGE
bool& S2_3 = expansion.attachInputBit(10, BUTTON_DEBOUNCE_CYCLES);            // VHOD 10 - S2.3 POZICIJA KORAKA DVIŽNE VERIGE
bool& S2_4 = expansion.attachInputBit(11, BUTTON_DEBOUNCE_CYCLES);            // VHOD 11 - S2.4 FLIPER SENZOR 1
bool& S2_5 = expansion.attachInputBit(12, BUTTON_DEBOUNCE_CYCLES);            // VHOD 12 - S2.5 FLIPER SENZOR 2
bool& S2_6 = expansion.attachInputBit(13, BUTTON_DEBOUNCE_CYCLES);            // VHOD 13 - S2.6 FLIPER SENZOR 3
bool& S2_7 = expansion.attachInputBit(14, BUTTON_DEBOUNCE_CYCLES);            // VHOD 14 - S2.7 FLIPER SENZOR 4
bool& S2_8 = expansion.attachInputBit(15, BUTTON_DEBOUNCE_CYCLES);            // VHOD 15 - S2.8 POZICIJA DESKE VMESNA POZICIJA

bool& S2_9 = expansion.attachInputBit(16, BUTTON_DEBOUNCE_CYCLES, INVERTED);  // VHOD 16 - S2.9 POZICIJA LESA KONEC

bool& S2_10 = expansion.attachInputBit(20, BUTTON_DEBOUNCE_CYCLES);           // VHOD 20 - S2.10 IZMET SLABIH OD (OSNOVNI POLOŽAJ)
bool& S2_11 = expansion.attachInputBit(21, BUTTON_DEBOUNCE_CYCLES);           // VHOD 21 - S2.11 IZMET SLABIH DP (DELOVNI POLOŽAJ)
bool& S2_13 = expansion.attachInputBit(22, BUTTON_DEBOUNCE_CYCLES);           // VHOD 22 - S2.13 IZMET DOBRIH OD (OSNOVNI POLOŽAJ)
bool& S2_14 = expansion.attachInputBit(23, BUTTON_DEBOUNCE_CYCLES);           // VHOD 23 - S2.14 IZMET DOBRIH DP (DELOVNI POLOŽAJ)



// OUTPUTS
bool& OUT_HOLD = expansion.attachOutputBit(0);  // IZHOD  0 - FREIGABE VKLOP
bool& ALARM = expansion.attachOutputBit(1);  // IZHOD  0 - ALARM

bool& M2_1 = expansion.attachOutputBit(8);      // IZHOD  8 - M2.1 VALJ?NA PROGA IZ SKOBELJNEGA
bool& M2_2 = expansion.attachOutputBit(9);      // IZHOD  9 - M2.2 IZMETALNA VERIGA
bool& M2_3 = expansion.attachOutputBit(10);     // IZHOD 10 - M2.3 DVIŽNA VERIGA
bool& M2_4 = expansion.attachOutputBit(11);     // IZHOD 11 - M2.4 VALJ?NA PROGA PRED SKENERJEM
bool& M2_5 = expansion.attachOutputBit(12);     // IZHOD 12 - M2.5 VALJ?NA PROGA ZA SKENERJEM

bool& Y2_1A = expansion.attachOutputBit(16);    // IZHOD 16 - Y2.1A FLIPER 1 OD (OSNOVNI POLOŽAJ)
bool& Y2_1B = expansion.attachOutputBit(17);    // IZHOD 17 - Y2.1B FLIPER 1 DP (DELOVNI POLOŽAJ)
bool& Y2_2A = expansion.attachOutputBit(18);    // IZHOD 18 - Y2.2A FLIPER 2 OD (OSNOVNI POLOŽAJ)
bool& Y2_2B = expansion.attachOutputBit(19);    // IZHOD 19 - Y2.2B FLIPER 2 DP (DELOVNI POLOŽAJ)
bool& Y2_3A = expansion.attachOutputBit(20);    // IZHOD 20 - Y2.3A IZMET SLABIH OD (OSNOVNI POLOŽAJ)
bool& Y2_3B = expansion.attachOutputBit(21);    // IZHOD 21 - Y2.3B IZMET SLABIH DP (DELOVNI POLOŽAJ)
bool& Y2_4A = expansion.attachOutputBit(22);    // IZHOD 22 - Y2.4A IZMET DOBRIH OD (OSNOVNI POLOŽAJ)
bool& Y2_4B = expansion.attachOutputBit(23);    // IZHOD 23 - Y2.4B IZMET DOBRIH DP (DELOVNI POLOŽAJ)

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
    void naprej() {
        Y2_4A = false;
        Y2_4B = true;
    }
    void nazaj() {
        Y2_4A = true;
        Y2_4B = false;
    }
    bool jeSpredaj() {
        return !S2_13 && S2_14;
    }
    bool jeZadaj() {
        return S2_13 && !S2_14;
    }
} IzmetacDobri;

bool P1 = false;  // Deska prisotna na koncu traka
bool P2 = true;   // Deska prisotna na odlagalnem mestu
bool P3 = true;   // Deska na zgornjem traku
bool P4 = true;   // Deska na vmesni poziciji
bool P5 = true;  // Deska na koncu traka

bool veriga_2_2_obratuje = false;

bool AUTO = false;   // Mode AUTO
bool ROCNO = false;  // Mode ROCNO
bool SERVIS = false; // Mode SERVIS
bool STOP = true;


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
        for (int i = velikost_zalogovnika - 1; i > 0; i--) pozicija[i] = pozicija[i - 1];
        pozicija[0] = 0;
        st_desk--;
        return output;
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
};