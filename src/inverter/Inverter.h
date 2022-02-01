#ifndef INVERTER_H
#define INVERTER_H

#include "hardware_api.h"
#include "../extra/movingAvg.h"

class Inverter{
    public:
        Inverter(int _GIVA, int _GIWA, int _VH, int _VL, int _CT, int _GIVT,  
                 int _CPWM, int _CSDN, int _GSDN, int _MSDN,                
                 int _GFIV, int _MFIV, int _OVH, int _OVL, int _FCV);        
    
        // function prototypes
        void init();                            // initialisation function
        void update();                          // an update loop used to monitor inverter signals
        void shutdown();                        // shutdown the whole inverter unit

        void shutdownConverter(bool isShutdown);// shutdown or enable the boost converter
        void shutdownMG1(bool isShutdown);      // shutdown or enable MG1
        void shutdownMG2(bool isShutdown);      // shutdown or enable MG2

        
        void setConverterPWM(float duty);
        
        float readVCurrent();                   // read the current on phase V of MG1
        float readWCurrent();                   // read the current on phase W of MG1
        float readCapacitorVoltage();           // read the inverter capacitor voltage
        float readConverterVoltage();           // read the boost converter input voltage
        float readConverterTemp();              // read the boost converter temperature
        float readInverterTemp();               // read MG1's temperature
        /** Driver disable function */
        //void disable();
        /** Driver enable function */
        //void enable();

        // Inverter signal pins
        int GIVA, GIWA, VH, VL, CT, GIVT;   // Analog signals to measure with an ADC
        int CPWM, CSDN, GSDN, MSDN;         // Signals to intercept and control
        int GFIV, MFIV, OVH, OVL, FCV;      // Digital error signals to monitor

        float VLOffset=-12.666, VHOffset=-17.8882;
        movingAvg VLAvg, VHAvg, CTAvg, GIVTAvg;
    
    private:
};

#endif