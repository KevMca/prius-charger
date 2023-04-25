#include "hardware_api.h"

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__) 

// Timer pins
#define OC0A 6
#define OC0B 5
#define OC1A 9
#define OC1B 10
#define OC2A 11
#define OC2B 3

int adcResolution = 10;     // number of resolution bits in the ADC
//float operatingVoltage = 5; // operating voltage of the ADC

bool _adcSetup() {
    // analog signal pin setup
    pinMode(GIVA, INPUT);
    pinMode(GIWA, INPUT);
    pinMode(VH,   INPUT);
    pinMode(VL,   INPUT);
    pinMode(CT,   INPUT);
    pinMode(GIVT, INPUT);

    // setup AdcExtern
    AdcExtern.begin(ADS1115_DEFAULT_ADDRESS);
    AdcExtern.setGain(ADS1115_REG_CONFIG_PGA_6_144V);    //Set the largest voltage range
    AdcExtern.setSampleRate(ADS1115_REG_CONFIG_DR_8SPS); //Set the slowest and most accurate sample rate

    //Test to see if you can communicate to the AdcExtern
    if (!AdcExtern.testConnection()) {
      Serial.println("AdcExtern Connection failed"); //oh man...something is wrong
      return 1;
    }

    return 0;
}

int _analogRead(int pin) {
    // If using internal ADC
    if(pin > 3) {
        return analogRead(pin);
    }
    // If using ADS1115 pins 0 to 3
    else {
        switch(pin) {
            case 0: {AdcExtern.setMux(ADS1115_REG_CONFIG_MUX_SINGLE_0);}
            break;
            case 1: {AdcExtern.setMux(ADS1115_REG_CONFIG_MUX_SINGLE_1);}
            break;
            case 2: {AdcExtern.setMux(ADS1115_REG_CONFIG_MUX_SINGLE_2);}
            break;
            case 3: {AdcExtern.setMux(ADS1115_REG_CONFIG_MUX_SINGLE_3);}
            break;
            default: {AdcExtern.setMux(ADS1115_REG_CONFIG_MUX_SINGLE_0);}
            break;
        }
        AdcExtern.triggerConversion();
        return AdcExtern.getConversion();
    }
}

float _readVcc() {
    long result;
    // Read 1.1V reference against AVcc
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Convert
    while (bit_is_set(ADCSRA, ADSC));
    result = ADCL;
    result |= ADCH << 8;
    result = 1126400L / result; // Calculate Vcc (in mV); 1126400 = 1.1*1024*1000
    return (float)result/1000;
}

void _converterPWMSetup() {
    // TCCRB
    // Set phase-correct PWM (WGM02=0) and prescaler to 8 (CS02=0, CS01=1, CS00=0)
    uint8_t TCCRB_set = bit(CS21);
    uint8_t TCCRB_clear = ~(bit(WGM22) | bit(CS22) | bit(CS20));
    uint8_t TCCRA_set;
    uint8_t TCCRA_clear;
    // TCCRA
    // Timer 0 A, Timer 1 A, Timer 2 A
    if (CPWM == OC0A || CPWM == OC1A || CPWM == OC2A) {  
        // Set to non-inverting mode (COM0A1=1, COM0A0=0) and phase-correct PWM (WGM00=1, WGM01=0)
        TCCRA_set = bit(COM0A1) | bit(WGM00);
        TCCRA_clear = ~(bit(COM0A0) | bit(WGM01));
    }
    // Timer 0 B, Timer 1 B, Timer 2 B
    if (CPWM == OC0B || CPWM == OC1B || CPWM == OC2B) {  
        // Set to non-inverting mode (COM0B1=1, COM0B0=0) and phase-correct PWM (WGM00=1, WGM01=0)
        TCCRA_set = bit(COM0B1) | bit(WGM00);
        TCCRA_clear = ~(bit(COM0B0) | bit(WGM01));
    }
    // Timer 0 
    if (CPWM == OC0A || CPWM == OC0B) {
        TCCR0A |= TCCRA_set;
        TCCR0A &= TCCRA_clear;
        TCCR0B |= TCCRB_set;
        TCCR0B &= TCCRB_clear;
    }
    // Timer 1
    if (CPWM == OC1A || CPWM == OC1B) {
        // Timer 1 (WGM11=1, WGM13=0) for 10-bit operation
        TCCR1A |= TCCRA_set | bit(WGM11);
        TCCR1A &= TCCRA_clear | bit(WGM11);
        TCCR1B |= bit(CS20);
        TCCR1B &= TCCRB_clear & ~(bit(CS21) | bit(WGM13)) | bit(CS20);
    }

    // Timer 2
    if (CPWM == OC2A || CPWM == OC2B) {
        TCCR2A |= TCCRA_set;
        TCCR2A &= TCCRA_clear;
        TCCR2B |= TCCRB_set;
        TCCR2B &= TCCRB_clear;
    }
}

void _converterPWMDuty(float duty) {
    // Timer 0
    if (CPWM == OC0A) {
        OCR0A = (uint8_t)(255*duty);
    } if (CPWM == OC0B) {
        OCR0B = (uint8_t)(255*duty);
    }
    // Timer 1
    if (CPWM == OC1A) {
        OCR1A = (uint16_t)(1023*duty);
    } if (CPWM == OC1B) {
        OCR1B = (uint16_t)(1023*duty);
    }
    // Timer 2
    if (CPWM == OC2A) {
        OCR2A = (uint8_t)(255*duty);
    } if (CPWM == OC2B) {
        OCR2B = (uint8_t)(255*duty);
    }
}

#endif