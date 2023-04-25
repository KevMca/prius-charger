#ifndef HAPI_INVERTER_H
#define HAPI_INVERTER_H

#include "Arduino.h"
#include "../extra/ADS1115.h"

bool _adcSetup();
int _analogRead(int pin);
float _readVcc();
void _converterPWMSetup();
void _converterPWMDuty(float duty);

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__) 

// analog signals
#define GIVA 0  // MG1 phase current V
#define GIWA 1  // MG1 phase current W
#define VH 3    // Inverter capacitor voltage
#define VL 2    // Boost converter input voltage
#define CT 14   // Boost converter temperature sensor
#define GIVT 15 // MG1 inverter temperature

// control signals
#define CPWM 3  // Boost converter switch signal
#define CSDN 2  // Boost converter shutdown signal
#define GSDN 4  // MG1 shutdown
#define MSDN 5  // MG2 shutdown

// digital errors
#define GFIV 10 // MG1 inverter fail
#define MFIV 11 // MG2 inverter fail
#define OVH 12  // Overvoltage
#define OVL 16  // Boost converter over voltage signal
#define FCV 17  // Boost converter fail signal

// Relay pins
#define BPRE 6  // Precharge relay
#define BPOS 7  // Positive battery relay
#define BNEG 8  // Negative battery relay
#define MAINS 9 // Mains relay

#endif // __AVR_ATmega328P__

#endif // HAPI_INVERTER_H