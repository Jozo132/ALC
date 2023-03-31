#pragma once
// JavaScript setInterval alternative
//
// Interval cycle(1000);
//
// if(cycle.check()) {
//    // do_something
// }
//
// // something_done 

#include <Arduino.h>

class Interval {
private:
    bool enabled = true;
    long last_trigger = 0;
    long duration = 1000;
    long t = 0;
public:
    Interval(long interval = 1000) {
        this->duration = interval;
    }
    bool check() {
        if (enabled) {
            t = millis();
            if (t < this->last_trigger || t >= this->last_trigger + this->duration) {
                this->last_trigger = t;
                return true;
            }
        }
        return false;
    }
    void reset() {
        this->last_trigger = millis();
    }
    void clear() {
        enabled = false;
    }
    void set() {
        enabled = true;
        this->reset();
    }
    void set(long interval) {
        this->duration = interval;
        enabled = true;
        this->reset();
    }
};

#ifdef USE_GLOBAL_INTERVAL_CHECK

Interval P_10s_timer(10000);
Interval P_1s_timer(1000);
Interval P_500ms_timer(500);
Interval P_300ms_timer(300);
Interval P_250ms_timer(250);
Interval P_200ms_timer(200);
Interval P_100ms_timer(100);
Interval P_50ms_timer(50);
Interval P_10ms_timer(10);

bool P_1day = false;
bool P_12hr = false;
bool P_6hr = false;
bool P_5hr = false;
bool P_4hr = false;
bool P_3hr = false;
bool P_2hr = false;
bool P_1hr = false;
bool P_30min = false;
bool P_15min = false;
bool P_10min = false;
bool P_5min = false;
bool P_1min = false;
bool P_30s = false;
bool P_10s = false;
bool P_1s = false;
bool P_500ms = false;
bool P_300ms = false;
bool P_250ms = false;
bool P_200ms = false;
bool P_100ms = false;
bool P_50ms = false;
bool P_10ms = false;

int P_1day_hour_cnt = 0;
int P_12hr_hour_cnt = 0;
int P_6hr_hour_cnt = 0;
int P_3hr_hour_cnt = 0;
int P_2hr_hour_cnt = 0;
int P_1hr_min_cnt = 0;
int P_30min_min_cnt = 0;
int P_15min_min_cnt = 0;
int P_10min_min_cnt = 0;
int P_5min_min_cnt = 0;
int P_1min_sec_cnt = 0;
int P_30s_sec_cnt = 0;


void IntervalGlobalLoopCheck() {
    P_1day = false;
    P_12hr = false;
    P_6hr = false;
    P_5hr = false;
    P_4hr = false;
    P_3hr = false;
    P_2hr = false;
    P_1hr = false;
    P_30min = false;
    P_15min = false;
    P_10min = false;
    P_5min = false;
    P_1min = false;
    P_30s = false;
    P_10s = P_10s_timer.check();
    P_1s = P_1s_timer.check();
    P_500ms = P_500ms_timer.check();
    P_300ms = P_300ms_timer.check();
    P_250ms = P_250ms_timer.check();
    P_200ms = P_200ms_timer.check();
    P_100ms = P_100ms_timer.check();
    P_50ms = P_50ms_timer.check();
    P_10ms = P_10ms_timer.check();
    if (P_1s) {
        P_1min_sec_cnt++;
        P_30s_sec_cnt++;
        if (P_30s_sec_cnt >= 30) {
            P_30s_sec_cnt = 0;
            P_30s = true;
        }
        if (P_1min_sec_cnt >= 60) {
            P_1min_sec_cnt = 0;
            P_1min = true;
        }
        if (P_1min) {
            P_5min_min_cnt++;
            P_10min_min_cnt++;
            P_15min_min_cnt++;
            P_30min_min_cnt++;
            P_1hr_min_cnt++;
            if (P_5min_min_cnt >= 5) {
                P_5min_min_cnt = 0;
                P_5min = true;
            }
            if (P_10min_min_cnt >= 10) {
                P_10min_min_cnt = 0;
                P_10min = true;
            }
            if (P_15min_min_cnt >= 15) {
                P_15min_min_cnt = 0;
                P_15min = true;
            }
            if (P_30min_min_cnt >= 30) {
                P_30min_min_cnt = 0;
                P_30min = true;
            }
            if (P_1hr_min_cnt >= 60) {
                P_1hr_min_cnt = 0;
                P_1hr = true;
            }
            if (P_1hr) {
                P_2hr_hour_cnt++;
                P_3hr_hour_cnt++;
                P_6hr_hour_cnt++;
                P_12hr_hour_cnt++;
                P_1day_hour_cnt++;
                if (P_2hr_hour_cnt >= 2) {
                    P_2hr_hour_cnt = 0;
                    P_2hr = true;
                }
                if (P_3hr_hour_cnt >= 3) {
                    P_3hr_hour_cnt = 0;
                    P_3hr = true;
                }
                if (P_6hr_hour_cnt >= 6) {
                    P_6hr_hour_cnt = 0;
                    P_6hr = true;
                }
                if (P_12hr_hour_cnt >= 12) {
                    P_12hr_hour_cnt = 0;
                    P_12hr = true;
                }
                if (P_1day_hour_cnt >= 24) {
                    P_1day_hour_cnt = 0;
                    P_1day = true;
                }
            }
        }
    }
}
#else
void IntervalGlobalLoopCheck() {
}
#endif