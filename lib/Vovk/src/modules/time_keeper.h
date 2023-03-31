#pragma once
#include <Arduino.h>

long _t = 0;
double t1 = 0;
double t0 = 0;
double dt = 0;
double t = 0;
bool validateTime() {
  t1 = millis();
  dt = (double) (t1 - t0) * 0.001;
  t0 = t1;
  if (dt < 0) return false;
  t += dt;
  return true;
}