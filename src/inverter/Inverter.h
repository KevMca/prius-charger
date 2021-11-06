#ifndef INVERTER_H
#define INVERTER_H

#include "hardware_api.h"

class Inverter{
    public:
        Inverter(int _GIVA, int _GIWA, int _VH, int _VL, int _CT, int _GIVT,  
                 int _CPWM, int _CSDN, int _GSDN, int _MSDN,                
                 int _GFIV, int _MFIV, int _OVH, int _OVL, int _FCV);        
    
        /** Initialisation function */
        void init();
        /** Driver disable function */
        void disable();
        /** Driver enable function */
        void enable();

        // Inverter signal pins
        int GIVA, int GIWA, int VH, int VL, int CT, int GIVT;   // Analog signals to measure with an ADC
        int CPWM, int CSDN, int GSDN, int MSDN;                 // Signals to intercept and control
        int GFIV, int MFIV, int OVH, int OVL, int FCV;          // Digital error signals to monitor
    
    private:
}

#endif