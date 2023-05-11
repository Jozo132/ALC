#pragma once
#include "runtime/runtime.h"

struct plc_t {
    bool IZKLOP_V_SILI = false;
    void izhodisce() {
        segment_2_1.izhodisce();
        segment_2_2.izhodisce();
        segment_2_3.izhodisce();
        segment_2_4.izhodisce();
        segment_2_5.izhodisce();
        segment_2_7.izhodisce();
    }
    void setup() {
        segment_2_1.setup(true);
        segment_2_2.setup(true);
        segment_2_3.setup(true);
        segment_2_4.setup(true);
        segment_2_5.setup(true);
        segment_2_7.setup(true);
        izhodisce();
    }

    void start() {
        segment_2_1.run();
        segment_2_2.run();
        segment_2_3.run();
        segment_2_4.run();
        segment_2_5.run();
        segment_2_7.run();
    }
    void stop_soft() {
        segment_2_1.stop();
        segment_2_2.stop_soft();
        segment_2_3.stop();
        segment_2_4.stop_soft();
        segment_2_5.stop();
        segment_2_7.stop();
    }
    void stop_hard() {
        stop_soft();
        segment_2_2.stop_hard();
        segment_2_4.stop_hard();
    }

    bool _old_stop = false;
    bool _old_auto = false;
    bool _old_rocno = false;
    bool _old_servis = false;

    bool _old_SW_1_M1 = false;
    bool _old_SW_2_C = false;
    bool _old_SW_3_M2 = false;
    bool _old_SW_4_M3 = false;

    void loop() {
        IZKLOP_V_SILI = EMERGENCY || IZPAD_BIMETALA;
        // IZKLOP_V_SILI = EMERGENCY;
        if (IZKLOP_V_SILI) {
            if (P_500ms) ALARM = !ALARM;
        } else ALARM = false;
        STOP = SW_A_STOP;
        AUTO = !STOP && !SW_MODE_1;
        ROCNO = !STOP && SW_MODE_1;
        SERVIS = false;

        bool START_PRESSED = SW_A_START /* || SW_BTN_MCU */;

        if (AUTO && _old_auto != AUTO) {
            // STOP = true;
            Serial.println("ACTIVATED AUTO");
        }
        if (ROCNO && _old_rocno != ROCNO) {
            // STOP = true;
            Serial.println("ACTIVATED ROCNO");
        }
        if (SERVIS && _old_servis != SERVIS) Serial.println("ACTIVATED SERVIS");
        if (STOP && _old_stop != STOP) Serial.println("ACTIVATED STOP");
        // if (_old_stop && !STOP) LED_AUTO = OFF;
        _old_stop = STOP;
        _old_auto = AUTO;
        _old_rocno = ROCNO;
        _old_servis = SERVIS;

        // if (!SERVIS || STOP) {
        //     SERVIS_M1 = false;
        //     SERVIS_C = false;
        //     SERVIS_M2 = false;
        //     SERVIS_M3 = false;
        // }



        bool all_stopped = !segment_2_1.running && !segment_2_2.running && !segment_2_3.running && !segment_2_4.running && !segment_2_5.running && !segment_2_7.running;

        // if (all_stopped) LED_AUTO = P_1s ? !LED_AUTO : LED_AUTO;
        // else LED_AUTO = ON;
        if (IZKLOP_V_SILI) {
            cycle_counter = 0;
            // SEMAFOR_RDECA = true;
            // SEMAFOR_ZELENA = false;
            stop_hard();
        } else if (STOP) {
            // SEMAFOR_RDECA = true;
            // SEMAFOR_ZELENA = false;
            if (SW_A_STOP)
                stop_hard();
            else
                stop_soft();
            P3 = true; // Forsiraj prisotnost deske
        } else {  // READY

            if (AUTO || ROCNO) {
                if (START_PRESSED) {
                    // LED_AUTO = true;
                    // SEMAFOR_RDECA = false;
                    // SEMAFOR_ZELENA = true;
                    start();
                }
                if (!all_stopped) {
                    idle_sec = t - last_idle_time;
                    if (idle_sec > 5) {
                        // SEMAFOR_RDECA = P_500ms ? !SEMAFOR_RDECA : SEMAFOR_RDECA;
                        // SEMAFOR_ZELENA = false;
                    } else {
                        // SEMAFOR_RDECA = false;
                        // SEMAFOR_ZELENA = true;
                    }
                } else {
                    // SEMAFOR_RDECA = true;
                    // SEMAFOR_ZELENA = false;
                }
            } else  if (SERVIS) {
                // SEMAFOR_RDECA = P_1s ? !SEMAFOR_RDECA : SEMAFOR_RDECA;
                // SEMAFOR_ZELENA = !SEMAFOR_RDECA;
                // if (SW_1_M1 && !_old_SW_1_M1) {
                //     SERVIS_M1 = !SERVIS_M1;
                //     MOTOR_1 = SERVIS_M1;
                // }
                // if (SW_2_C && !_old_SW_2_C) {
                //     SERVIS_C = !SERVIS_C;
                //     if (SERVIS_C) {
                //         segment_2_1.naprej(2);
                //     } else {
                //         segment_2_1.nazaj(2);
                //     }
                // }
                // if (SW_3_M2 && !_old_SW_3_M2) {
                //     SERVIS_M2 = !SERVIS_M2;
                //     if (SERVIS_M2) {
                //         segment_2_5.run();
                //     }
                // }
                // if (SW_4_M3 && !_old_SW_4_M3) {
                //     SERVIS_M3 = !SERVIS_M3;
                //     MOTOR_3 = SERVIS_M3;
                // }
            }
            // if (ROCNO && SW_R_CYCLE) start();

            if (SW_IZHODISCE) {
                izhodisce();
            }
        }

        // LED_1_M1 = MOTOR_1;
        // LED_2_C = !C1_zadaj;
        // LED_3_M2 = MOTOR_2;
        // LED_4_M3 = MOTOR_3;

        // OUT_HOLD = !MOTOR_1;

        // _old_SW_1_M1 = SW_1_M1;
        // _old_SW_2_C = SW_2_C;
        // _old_SW_3_M2 = SW_3_M2;
        // _old_SW_4_M3 = SW_4_M3;

        if (P_5s) {
            Serial.printf("PLC status: P1: %d , P2: %d , P3: %d , P4: %d , P5: %d , P6: %d\n",
                P1, P2, P3, P4, P5, P6
            );
        }


        if (!SERVIS) {
            segment_2_1.loop();
            segment_2_3.loop();
            segment_2_5.loop();
            segment_2_7.loop();
        }
        segment_2_2.loop();
        segment_2_4.loop();

        // TODO: Izmet slabih 
        Y2_3A = true;
        Y2_3B = false;

        // IO Latency test for debounce
        // Y_TEST = S_TEST;

        Fliper.sync();
    }
};

plc_t plc;