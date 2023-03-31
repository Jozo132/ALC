#pragma once
#include <Arduino.h>
#include "flow_manager.h"
#include "timeout.h"

struct _vovk_plc_block_t {
    bool enabled = false;
    bool running = false;
    bool stopping = false;
    bool finished = false;
    bool safe = false;

    
    int errors_count[10] = { 0 };
    bool errors[10] = { false };
    void setup();
    Timeout timer;
    FlowManager flow;
};