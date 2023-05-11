#include <PLC.h>

void setup() {
    IWatchdog.begin(30000);
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
    long cycle_start_us = micros();
    cycle_count++;
    validateTime();
    P_5s = P_5s_timer.check();
    P_1s = P_1s_timer.check();
    P_500ms = P_500ms_timer.check();
    P_200ms = P_200ms_timer.check();
    P_100ms = P_100ms_timer.check();
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
    if (P_5s) Serial.printf("Cycle time [us]: %d\n", cycleTime_us);
    cycleTime_us = micros() - cycle_start_us;
    IWatchdog.reload();
}

// 1kHz Parallel running code
void thread(void) {
}
