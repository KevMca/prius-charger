#include "Inverter.h"

Inverter::Inverter(){
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

}

// initialise pwm pins
void Inverter::init() {
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
}