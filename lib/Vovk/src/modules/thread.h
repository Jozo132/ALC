#pragma once
#include <Arduino.h>

void thread(void);

static HardwareTimer thread_Timer(TIM2);
int thread_Timer_Number = 2;

long thread_HAL_time = 0;
long thread_NEW_time = 0;

bool thread_busy = false;
void thread_loop() {
    if (!thread_busy) {
        thread_busy = true;
        thread();
        thread_busy = false;
    }
}

void thread_setup(uint32_t frequency = 1) {
    thread_Timer.pause();
    //thread_Timer.setPrescaleFactor(1680 - 1); // 1680-1 => 10us tick
    thread_Timer.setPrescaleFactor(1); // 1680-1 => 10us tick
    thread_Timer.setMode(thread_Timer_Number, TIMER_OUTPUT_COMPARE_TOGGLE);
    thread_Timer.setOverflow(frequency, HERTZ_FORMAT);
    thread_Timer.attachInterrupt(thread_Timer_Number, thread_loop); // channel 2, handler
    thread_Timer.resume();
}