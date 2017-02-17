/*
 *MayFlyOnboardSensors.cpp
 *This file is part of the EnviroDIY modular sensors library for Arduino
 *
 *Work in progress by Sara Damiano taken from code written
 *by Shannon Hicks and templates from USU.
 *
 *This file is for the onboard "sensors" on the EnviroDIY Mayfly
 *It is dependent on the EnviroDIY DS3231 library.
*/

#include "Arduino.h"
#include "Sensor.h"
#include "MayFlyOnboardSensors.h"
#include <Sodaq_DS3231.h>



// The constructor - what happens when initializing the onboard sensors
// only need to tell it the battery pin
MayFlyOnboardSensors::MayFlyOnboardSensors(void)
{
    _batteryPin = A6;
}

// The destructor - leave empty
MayFlyOnboardSensors::~MayFlyOnboardSensors(void) {}

// The sensor name
String MayFlyOnboardSensors::getSensorName(void)
{
    sensorName = F("EnviroDIY Mayfly");
    return sensorName;
}

// How to update the onboard sensors
bool MayFlyOnboardSensors::update(void)
{
    // Get the temperature from the Mayfly's real time clock
    rtc.convertTemperature();  //convert current temperature into registers
    float tempVal = rtc.getTemperature();
    sensorValue_temp = tempVal;

    // Get the battery voltage
    float rawBattery = analogRead(_batteryPin);
    sensorValue_battery = (3.3 / 1023.) * 1.47 * rawBattery;

    // Return true when finished
    Serial.print("updated sensor");
    return true;
}



MayFlyOnboardTemp::MayFlyOnboardTemp(void) {}
MayFlyOnboardTemp::~MayFlyOnboardTemp(void) {}

String MayFlyOnboardTemp::getVarName(void)
{
    varName = F("temperatureDatalogger");
    return varName;
}

String MayFlyOnboardTemp::getVarUnit(void)
{
    String unit = F("degreeCelsius");
    return unit;
}

float MayFlyOnboardTemp::getValue(void)
{
    return sensorValue_temp;
}


MayFlyOnboardBatt::MayFlyOnboardBatt(void) {}
MayFlyOnboardBatt::~MayFlyOnboardBatt(void) {}
String MayFlyOnboardBatt::getVarName(void)
{
    varName = F("batteryVoltage");
    return varName;
}


String MayFlyOnboardBatt::getVarUnit(void)
{
    unit = F("Volt");
    return unit;
}

float MayFlyOnboardBatt::getValue(void)
{
    return sensorValue_battery;
}
