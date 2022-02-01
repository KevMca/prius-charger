#ifndef HAPI_INVERTER_H
#define HAPI_INVERTER_H

#include "Arduino.h"

float _adcToVoltage(int val);
void _converterPWMSetup(int CPWM);
void _converterPWMDuty(float duty, int CPWM);

#endif