/* prius-charger.ino
 * A sketch designed to control the Toyota Prius Gen 2 (2004) inverter to
 * charge batteries from an AC source.
 *
 * Date created: 06/11/2021
 * Author: Kevin McAndrew
 *
*/

#include <PriusCharger.h>

Inverter inverter = Inverter();

void inverterSummary();

void setup() {
  // setup serial
  Serial.begin(9600);
  // initialise the driver object
  inverter.init();
}


void loop() {
  inverter.activateBatteryPositive(true);
  delay(500);
  inverter.activateBatteryNegative(true);
  delay(500);
  inverter.activateBatteryPrecharge(true);
  delay(500);
  inverter.activateBatteryPositive(false);
  delay(500);
  inverter.activateBatteryNegative(false);
  delay(500);
  inverter.activateBatteryPrecharge(false);
  delay(500);
}

void inverterSummary(){
  // Print all of the available measurements from the inverter
  Serial.println("----------------------------------------");
  Serial.println("MG1 phase V I|MG1 phase W I|Capacitor V|Converter V|Converter Temp|MG1 Temp");
  printf( "%13.4f %13.4f %11.4f %11.4f %14.4f %7.4f\n", 
    inverter.readVCurrent(), inverter.readWCurrent(), inverter.readCapacitorVoltage(), 
    inverter.readConverterVoltage(), inverter.readConverterTemp(), inverter.readInverterTemp());
}
