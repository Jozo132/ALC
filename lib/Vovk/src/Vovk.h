#pragma once
#include <Arduino.h>

#ifdef USER_BTN
#undef USER_BTN
#define USER_BTN PA0
#endif

#include "modules/std.h"
#include "modules/auto_pins.h"
#include "modules/debounce.h"
#include "modules/differentiate.h"
#include "modules/flow_manager.h"
#include "modules/interval.h"
#include "modules/long_press.h"
#include "modules/plc.h"
#include "modules/thread.h"
#include "modules/timeout.h"
#include "modules/time_keeper.h"