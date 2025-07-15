#pragma once

#include <Arduino.h>
#include <IWatchdog.h>
#include <Vovk.h>

#ifndef XTP_14A6_E
Interval P_5s_timer(5000);
Interval P_1s_timer(1000);
Interval P_500ms_timer(500);
Interval P_200ms_timer(200);
Interval P_100ms_timer(100);
Interval P_50ms_timer(50);

bool P_5s = false;
bool P_1s = false;
bool P_500ms = false;
bool P_200ms = false;
bool P_100ms = false;
bool P_50ms = false;

#endif

#include "./programs/Segment 2-1 proga.h"
#include "./programs/Segment 2-2 veriga.h"
#include "./programs/Segment 2-3 fliper.h"
#include "./programs/Segment 2-4 veriga.h"
#include "./programs/Segment 2-5 proga.h"
#include "./programs/Segment 2-7 izmet dobri.h"
#include "config.h"