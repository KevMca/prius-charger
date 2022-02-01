#include "Inverter.h"

Inverter::Inverter(
    int _GIVA, int _GIWA, int _VH, int _VL, int _CT, int _GIVT,  
    int _CPWM, int _CSDN, int _GSDN, int _MSDN,                
    int _GFIV, int _MFIV, int _OVH, int _OVL, int _FCV
)
: VLAvg(10), VHAvg(10), CTAvg(10), GIVTAvg(10)
{
    // analog signal pin initialisation
    GIVA = _GIVA;
    GIWA = _GIWA;
    VH = _VH;
    VL = _VL;
    CT = _CT;
    GIVT = _GIVT;
    
    // control signal pin initialisation
    CPWM = _CPWM;
    CSDN = _CSDN;
    GSDN = _GSDN;
    MSDN = _MSDN;
    
    // digital error signal pin initialisation
    GFIV = _GFIV;
    MFIV = _MFIV;
    OVH = _OVH;
    OVL = _OVL;
    FCV = _FCV;

    VLAvg.begin();
    VHAvg.begin();
    CTAvg.begin();
    GIVTAvg.begin();
}

// initialise pwm pins
void Inverter::init() {
    // set analog reference voltage to 5.1V from zener diode on AREF
    analogReference(EXTERNAL);

    // analog signal pin setup
    pinMode(GIVA, INPUT);
    pinMode(GIWA, INPUT);
    pinMode(VH,   INPUT);
    pinMode(VL,   INPUT);
    pinMode(CT,   INPUT);
    pinMode(GIVT, INPUT);

    // control signal pin setup
    pinMode(CPWM, OUTPUT);
    pinMode(CSDN, OUTPUT);
    pinMode(GSDN, OUTPUT);
    pinMode(MSDN, OUTPUT);

    // digital signal pin setup
    pinMode(GFIV, INPUT);
    pinMode(MFIV, INPUT);
    pinMode(OVH,  INPUT);
    pinMode(OVL,  INPUT);
    pinMode(FCV,  INPUT);

    // set converter PWM
    // enable converter PWM
    _converterPWMSetup(CPWM);
    setConverterPWM(0.95);
}

// loop update function for monitoring signals
void Inverter::update() {

}

// loop update function for monitoring signals
void Inverter::shutdown() {
    shutdownConverter(true);
    shutdownMG1(true);
    shutdownMG2(true);

    // Turn off relays also
}

// shutdown or enable the boost converter
void Inverter::shutdownConverter(bool isShutdown) {
    if (isShutdown == true){
        digitalWrite(CSDN, LOW);
    } else if (isShutdown == false) {
        digitalWrite(CSDN, HIGH);
    }
}

// shutdown or enable the inverter signals for MG1
void Inverter::shutdownMG1(bool isShutdown) {
    if (isShutdown == true){
        digitalWrite(GSDN, LOW);
    } else if (isShutdown == false) {
        digitalWrite(GSDN, HIGH);
    }
}

// shutdown or enable the inverter signals for MG1
void Inverter::shutdownMG2(bool isShutdown) {
    if (isShutdown == true){
        digitalWrite(MSDN, LOW);
    } else if (isShutdown == false) {
        digitalWrite(MSDN, HIGH);
    }
}

// set the converter PWM duty defined as a value from 0 to 1
void Inverter::setConverterPWM(float duty) {
    // Should be hardware specific
    _converterPWMDuty(duty, CPWM);
}

// read the current on phase V of MG1
float Inverter::readVCurrent() {
    int    val     = analogRead(GIVA);
    float  voltage = _adcToVoltage(val);    // hardware specific function
    return voltage;
}

// read the current on phase W of MG1
float Inverter::readWCurrent() {
    int    val = analogRead(GIWA);
    float  voltage = _adcToVoltage(val);    // hardware specific function
    return voltage;
}

// read the inverter capacitor voltage
float Inverter::readCapacitorVoltage() {
    int    val = analogRead(VH);
    VHAvg.reading(val);
    int    valAvg  = VHAvg.getAvg();
    float  voltage = _adcToVoltage(valAvg);
    return ((voltage-0.5)*100) - VHOffset;  // Vout = (v-0.5)*100
}

// read the boost converter input voltage
float Inverter::readConverterVoltage() {
    int    val = analogRead(VL);
    VLAvg.reading(val);
    int    valAvg = VLAvg.getAvg();
    float  voltage = _adcToVoltage(valAvg);
    return ((voltage-0.5)*100) - VLOffset;  // Vout = (v-0.5)*100
}

// read the boost converter temperature
float Inverter::readConverterTemp() {
    int    val = analogRead(CT);
    CTAvg.reading(val);
    int valAvg = CTAvg.getAvg();
    float  voltage = _adcToVoltage(valAvg); // hardware specific function
    //return ((5-voltage)/0.03);              // T = (5V-v)/0.03
    return voltage;
}

// read MG1's temperature
float Inverter::readInverterTemp() {
    int    val = analogRead(GIVT);
    GIVTAvg.reading(val);
    int valAvg = GIVTAvg.getAvg();
    float  voltage = _adcToVoltage(valAvg); // hardware specific function
    //return ((5-voltage)/0.03);              // T = (5V-v)/0.03
    return voltage;
}
