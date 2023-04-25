#ifndef INVERTER_H
#define INVERTER_H

#include "hardware_api.h"
#include "../extra/movingAvg.h"

//Different states of charger
typedef enum
{
    Idle_State,
    Precharge_start_State,
    Step_up_State,
    Charge_start_State,
    Constant_current_State,
    Constant_voltage_State,
    Charge_end_State,
    Error_State
} InverterState;

//Different events of charger
typedef enum
{
    Start_Event,
    Precharged_Event,
    Stepped_up_Event,
    Zero_current_Event,
    Voltage_threshold_Event,
    Charged_Event,
    Overvoltage_Event,
    Overcurrent_Event,
    Undervoltage_Event,
    Overtemperature_Event,
    Timeout_Event
} InverterEvent;

class Inverter{
    private:
        float adcToVoltage(int val, int adcResolution, float maxVoltage);

        // Charging parameters
        float targetBatteryVoltage = 12.4;  // The target voltage to charge the battery to
        float targetMainsVoltage = 24.0;    // The target capacitor voltage before connecting the mains
        float zeroCurrentThreshold = 0.1;   // Minimum current flowing to be considered 0 current

        float currentDuty = 0;
        InverterState currentState = Idle_State;

    public:
        Inverter();         
    
        // function prototypes
        int  init();                                    // initialisation function
        void update();                                  // an update loop used to monitor inverter signals
        void shutdown();                                // shutdown the whole inverter unit

        void shutdownConverter(bool isShutdown);        // shutdown or enable the boost converter
        void shutdownMG1(bool isShutdown);              // shutdown or enable MG1
        void shutdownMG2(bool isShutdown);              // shutdown or enable MG2
        void activateBatteryPositive(bool isActive);    // activate or deactivate battery positive relay
        void activateBatteryNegative(bool isActive);    // activate or deactivate battery negative relay
        void activateBatteryPrecharge(bool isActive);   // activate or deactivate battery precharge relay
        void activateMains(bool isActive);              // activate or deactivate the mains positive and negative relays

        void setConverterPWM(float duty);
        
        float readVCurrent();                           // read the current on phase V of MG1
        float readWCurrent();                           // read the current on phase W of MG1
        float readCapacitorVoltage();                   // read the inverter capacitor voltage
        float readConverterVoltage();                   // read the boost converter input voltage
        float readConverterTemp();                      // read the boost converter temperature
        float readInverterTemp();                       // read MG1's temperature
        /** Driver disable function */
        //void disable();
        /** Driver enable function */
        //void enable();

        int adcResolution = 16;
        int adcMaxVoltage = 6.144;
    
    private:
};

#endif