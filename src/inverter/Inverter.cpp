#include "Inverter.h"

Inverter::Inverter() {}

// initialise pwm pins
int Inverter::init() {
    int err;

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

    // relay pin setup
    pinMode(BPRE, OUTPUT);
    pinMode(BPOS, OUTPUT);
    pinMode(BNEG,  OUTPUT);
    pinMode(MAINS,  OUTPUT);

    // setup adc
    err = _adcSetup();
    if(err != 0) {
        return err;
    }

    // set converter PWM
    // enable converter PWM
    _converterPWMSetup();
    setConverterPWM(currentDuty);
}

// loop update function for monitoring signals
void Inverter::update() {
    // Read any new events
    //InverterEvent currentEvent = readEvent();

    // If time between updates is too large, timeout has occured
        // currentState = Error_State

    // Use interrupts for overvoltage, undervoltage, overtemperature and overcurrent faults!

    switch(currentState) {
        case Idle_State:{
            // Turn off all contactors and converter / inverter
            shutdown();
            // If the start command is given
            if (Serial.available() > 0) {
                // Move to precharge state
                currentState = Precharge_start_State;
            }
        }
        break;
        case Precharge_start_State:{
            // Turn on precharge battery contactor and negative battery contactor
            activateBatteryPositive(false);
            activateBatteryNegative(true);
            activateBatteryPrecharge(true);
            activateMains(false);
            // If the capacitor voltage has stabilised
            if(abs(readCapacitorVoltage() - readConverterVoltage()) < 1) {
                // Move to step up state
                currentState = Step_up_State;
            }
        }
        break;
        /*
        case Step_up_State:{
            // Turn on main battery contactors
            activateBatteryPositive(true);
            activateBatteryNegative(true);
            activateBatteryPrecharge(false);
            activateMains(false);
            // If the capacitor voltage is at the correct voltage
            if(abs(readCapacitorVoltage() - targetMainsVoltage) < 1)
                Move to charge start state
                currentState = Charge_start_State;
            else {
                // Increment the PWM duty cycle
                setConverterPWM(currentDuty + someTerm);
            }
        }
        break;
        case Charge_start_State:{
            // Turn on main battery contactors and mains contactors
            activateBatteryPositive(true);
            activateBatteryNegative(true);
            activateBatteryPrecharge(false);
            activateMains(true);
            // If the current is at zero
            if(readVCurrent < zeroCurrentThreshold) {
                // Move to the constant current state
                currentState = Constant_current_State;
            } else {
                // Adjust the PWM duty cycle
                setConverterPWM(currentDuty + someTerm);
            }
        }
        break;
        case Constant_current_State:{
            // Turn on main battery contactors and mains contactors
            activateBatteryPositive(true);
            activateBatteryNegative(true);
            activateBatteryPrecharge(false);
            activateMains(true);
            // If the voltage threshold has been reached
            if(abs(readConverterVoltage() - targetBatteryVoltage) < 1)
                // Move to the constant voltage state
                currentState = Constant_voltage_State
            } else {
                // Continue monitoring constant current
                setConverterPWM(currentDuty + someTerm);
            }
        }
        break;
        case Constant_voltage_State:{
            // Turn on main battery contactors and mains contactors
            activateBatteryPositive(true);
            activateBatteryNegative(true);
            activateBatteryPrecharge(false);
            activateMains(true);
            // If the charged voltage threshold has been reached
            if(abs(readConverterVoltage() - targetBatteryVoltage) < 1 && (readVCurrent < zeroCurrentThreshold))
                // Move to the charge end state
                currentState = Charge_end_State
            } else {
                // Continue monitoring constant voltage
                setConverterPWM(currentDuty + someTerm);
            }
        }
        break;
        case Charge_end_State:{
            // Turn on main battery contactors and mains contactors
            activateBatteryPositive(true);
            activateBatteryNegative(true);
            activateBatteryPrecharge(false);
            activateMains(true);
            // If the current is at zero
            if(readVCurrent < zeroCurrentThreshold) {
                // Move to the idle state
                currentState = Idle_State
            } else {
                // Reduce the PWM duty cycle
                setConverterPWM(currentDuty + someTerm);
            }
        }
        break;
        */
        default:
            break;
    }
}

// shutdown the converter, inverter and disconnect external relays
void Inverter::shutdown() {
    shutdownConverter(true);
    shutdownMG1(true);
    shutdownMG2(true);

    // Turn off relays also
    activateBatteryPositive(false);
    activateBatteryNegative(false);
    activateBatteryPrecharge(false);
    activateMains(false);
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

// activate or deactivate the battery positive relay
void Inverter::activateBatteryPositive(bool isActive) {
    if (isActive == true){
        digitalWrite(BPOS, HIGH);
    } else if (isActive == false) {
        digitalWrite(BPOS, LOW);
    }
}

// activate or deactivate the battery negative relay
void Inverter::activateBatteryNegative(bool isActive) {
    if (isActive == true){
        digitalWrite(BNEG, HIGH);
    } else if (isActive == false) {
        digitalWrite(BNEG, LOW);
    }
}

// activate or deactivate the battery precharge relay
void Inverter::activateBatteryPrecharge(bool isActive) {
    if (isActive == true){
        digitalWrite(BPRE, HIGH);
    } else if (isActive == false) {
        digitalWrite(BPRE, LOW);
    }
}

// activate or deactivate the mains positive and negative relays
void Inverter::activateMains(bool isActive) {
    if (isActive == true){
        digitalWrite(MAINS, HIGH);
    } else if (isActive == false) {
        digitalWrite(MAINS, LOW);
    }
}

// set the converter PWM duty defined as a value from 0 to 1
void Inverter::setConverterPWM(float duty) {
    // Should be hardware specific
    _converterPWMDuty(duty);
}

// read the current on phase V of MG1
float Inverter::readVCurrent() {
    int    val     = _analogRead(GIVA);
    float  voltage = adcToVoltage(val, adcResolution, adcMaxVoltage);
    return voltage;
}

// read the current on phase W of MG1
float Inverter::readWCurrent() {
    int    val = analogRead(GIWA);
    float  voltage = adcToVoltage(val, adcResolution, adcMaxVoltage);
    return voltage;
}

// read the inverter capacitor voltage
float Inverter::readCapacitorVoltage() {
    int    val = _analogRead(VH);
    float  voltage = adcToVoltage(val, adcResolution, adcMaxVoltage);
    return ((voltage-0.5)*100);
}

// read the boost converter input voltage
float Inverter::readConverterVoltage() {
    int    val = _analogRead(VL);
    float  voltage = adcToVoltage(val, adcResolution, adcMaxVoltage);
    return ((voltage-0.5)*100);
}

// read the boost converter temperature
float Inverter::readConverterTemp() {
    int    val = _analogRead(CT);
    float  voltage = adcToVoltage(val, adcResolution, _readVcc());
    return voltage;//((5-voltage)/0.03);      // T = (5V-v)/0.03
}

// read MG1's temperature
float Inverter::readInverterTemp() {
    int    val = _analogRead(GIVT);
    float  voltage = adcToVoltage(val, adcResolution, _readVcc());
    return voltage;//((5-voltage)/0.03);      // T = (5V-v)/0.03
}

// converts an adc value to a voltage, given the maximum voltage
float Inverter::adcToVoltage(int val, int adcResolution, float maxVoltage) {
    float decimal = (float)val/(float)pow(2,adcResolution-1);
    float voltage = decimal*maxVoltage;
    return voltage;
}
