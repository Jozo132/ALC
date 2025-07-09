#ifdef XTP_14A6_E
#include <xtp-lib.h>
#endif

#include <PLC.h>
void thread();

void setup() {
#ifdef XTP_14A6_E
    xtp_setup();
#endif
    IWatchdog.begin(15'000'000);
    IWatchdog.reload();
    Serial.begin(115200);
    LED = OFF;
    AutoPins.loop();
    for (int i = 0; i < 5; i++) {
        LED = ON;
        AutoPins.loop();
        smartDelay(50);
        LED = OFF;
        AutoPins.loop();
        smartDelay(50);
    }
    expansion.setSPI(sck_pin, miso_pin, mosi_pin);
    expansion.begin();
    // thread_setup(1000);
    smartDelay(1000);
    IWatchdog.reload();
    smartDelay(1000);
    plc.setup();
    IWatchdog.reload();
    validateTime();
    P_1s_timer.reset();
    P_500ms_timer.reset();
    P_200ms_timer.reset();
    P_100ms_timer.reset();
    IWatchdog.reload();
    thread_setup(1'000, thread);
}

uint32_t cycle_count = 0;

void log_bytes(const char* name, const byte* bytes, int len, bool reverse = false) {
    Serial.printf("[%d] %s: ", cycle_count, name);
    for (int i = 0; i < len; i++) {
        if (reverse) for (int b = 7; b >= 0; b--) Serial.print((bytes[i] >> b) & 1);
        else for (int b = 0; b < 8; b++) Serial.print((bytes[i] >> b) & 1);
        Serial.print(" ");
    }
    Serial.println();
}
int sec_count = 0;
void loop() {
#ifdef XTP_14A6_E
    xtp_loop();
#endif

    if (P_50ms) {
        char msg[128];
        sprintf(msg, "Time: %d us", cycleTime_us);
        Serial.printf("%s\n", msg);
        auto& I = expansion.input;
        auto& O = expansion.output;
        int i = 0;
        int o = 0;


        sprintf(msg, "I:%c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c",
            (I[i] >> 0) & 1 ? '1' : '-', (I[i] >> 1) & 1 ? '1' : '-', (I[i] >> 2) & 1 ? '1' : '-', (I[i] >> 3) & 1 ? '1' : '-',
            (I[i] >> 4) & 1 ? '1' : '-', (I[i] >> 5) & 1 ? '1' : '-', (I[i] >> 6) & 1 ? '1' : '-', (I[i++] >> 7) & 1 ? '1' : '-',
            (I[i] >> 0) & 1 ? '1' : '-', (I[i] >> 1) & 1 ? '1' : '-', (I[i] >> 2) & 1 ? '1' : '-', (I[i] >> 3) & 1 ? '1' : '-',
            (I[i] >> 4) & 1 ? '1' : '-', (I[i] >> 5) & 1 ? '1' : '-', (I[i] >> 6) & 1 ? '1' : '-', (I[i++] >> 7) & 1 ? '1' : '-');
        oled_print(msg, 0, 0);
        sprintf(msg, "  %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c",
            (I[i] >> 0) & 1 ? '1' : '-', (I[i] >> 1) & 1 ? '1' : '-', (I[i] >> 2) & 1 ? '1' : '-', (I[i] >> 3) & 1 ? '1' : '-',
            (I[i] >> 4) & 1 ? '1' : '-', (I[i] >> 5) & 1 ? '1' : '-', (I[i] >> 6) & 1 ? '1' : '-', (I[i++] >> 7) & 1 ? '1' : '-',
            (I[i] >> 0) & 1 ? '1' : '-', (I[i] >> 1) & 1 ? '1' : '-', (I[i] >> 2) & 1 ? '1' : '-', (I[i] >> 3) & 1 ? '1' : '-',
            (I[i] >> 4) & 1 ? '1' : '-', (I[i] >> 5) & 1 ? '1' : '-', (I[i] >> 6) & 1 ? '1' : '-', (I[i++] >> 7) & 1 ? '1' : '-');
        oled_print(msg, 0, 1);

        sprintf(msg, "O:%c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c",
            (O[o] >> 0) & 1 ? '1' : '-', (O[o] >> 1) & 1 ? '1' : '-', (O[o] >> 2) & 1 ? '1' : '-', (O[o] >> 3) & 1 ? '1' : '-',
            (O[o] >> 4) & 1 ? '1' : '-', (O[o] >> 5) & 1 ? '1' : '-', (O[o] >> 6) & 1 ? '1' : '-', (O[o++] >> 7) & 1 ? '1' : '-',
            (O[o] >> 0) & 1 ? '1' : '-', (O[o] >> 1) & 1 ? '1' : '-', (O[o] >> 2) & 1 ? '1' : '-', (O[o] >> 3) & 1 ? '1' : '-',
            (O[o] >> 4) & 1 ? '1' : '-', (O[o] >> 5) & 1 ? '1' : '-', (O[o] >> 6) & 1 ? '1' : '-', (O[o++] >> 7) & 1 ? '1' : '-');
        oled_print(msg, 0, 3);
        sprintf(msg, "  %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c",
            (O[o] >> 0) & 1 ? '1' : '-', (O[o] >> 1) & 1 ? '1' : '-', (O[o] >> 2) & 1 ? '1' : '-', (O[o] >> 3) & 1 ? '1' : '-',
            (O[o] >> 4) & 1 ? '1' : '-', (O[o] >> 5) & 1 ? '1' : '-', (O[o] >> 6) & 1 ? '1' : '-', (O[o++] >> 7) & 1 ? '1' : '-',
            (O[o] >> 0) & 1 ? '1' : '-', (O[o] >> 1) & 1 ? '1' : '-', (O[o] >> 2) & 1 ? '1' : '-', (O[o] >> 3) & 1 ? '1' : '-',
            (O[o] >> 4) & 1 ? '1' : '-', (O[o] >> 5) & 1 ? '1' : '-', (O[o] >> 6) & 1 ? '1' : '-', (O[o++] >> 7) & 1 ? '1' : '-');
        oled_print(msg, 0, 4);


    }
    IWatchdog.reload();
}

// 1kHz Parallel running code
void thread(void) {
    long cycle_start_us = micros();
    cycle_count++;
    validateTime();
#ifndef XTP_14A6_E
    P_5s = P_5s_timer.check();
    P_1s = P_1s_timer.check();
    P_500ms = P_500ms_timer.check();
    P_200ms = P_200ms_timer.check();
    P_100ms = P_100ms_timer.check();
#else // XTP_14A6_E
    IntervalGlobalLoopCheck();
#endif
    AutoPins.loop();
    expansion.loop();
    if (P_1s) sec_count++;
    if (P_100ms) LED = !LED;
    if (P_1s && sec_count % 10 == 0) {
        // int size = expansion.availableBytes();
        // byte* inputs = expansion.readBytes();
        // byte* outputs = expansion.readOutputBytes();
        // log_bytes("LSR32IO inputs", inputs, size);
        // log_bytes("LSR32IO outputs", outputs, size);
        int fliper_pos_target = Fliper.current_position;
        int fliper_pos_actual = Fliper.getPosition();
        // Serial.printf("[%d] Fliper target: %d, actual: %d\n", cycle_count, fliper_pos_target, fliper_pos_actual);

        int flow_phase_2_2 = segment_2_2.flow.phase;
    }
    plc.loop();
    AutoPins.loop();
    expansion.loop();
    cycleTime_us = micros() - cycle_start_us;
}
