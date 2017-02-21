/*
 *MayflyOnboardSensors.h
 *This file is part of the EnviroDIY modular sensors library for Arduino
 *
 *Work in progress by Sara Damiano taken from code written
 *by Shannon Hicks and templates from USU.
 *
 *This file is for the onboard "sensors" on the EnviroDIY Mayfly
 *It is dependent on the EnviroDIY DS3231 library.
*/

#ifndef MayflyOnboardSensors_h
#define MayflyOnboardSensors_h

#include <Arduino.h>
#include "SensorBase.h"

// The main class for the Mayfly
class MayflyOnboardSensors : public virtual SensorBase
{
public:
    MayflyOnboardSensors(int batteryPin);

    bool update(void) override;
    String getSensorName(void) override;
    String getSensorLocation(void) override;

    virtual String getVarName(void) = 0;
    virtual String getVarUnit(void) = 0;
    virtual float getValue(void) = 0;
    virtual String getDreamHost(void) = 0;
protected:
    SENSOR_STATUS sensorStatus;
    String sensorName;
    String sensorLocation;
    String varName;
    String unit;
    int _batteryPin;
    static float sensorValue_temp;
    static float sensorValue_battery;
};


// Defines the "Temperature Sensor"
class MayflyOnboardTemp : public virtual MayflyOnboardSensors
{
public:
    MayflyOnboardTemp(int batteryPin);

    String getVarName(void) override;
    String getVarUnit(void) override;
    float getValue(void) override;
    String getDreamHost(void) override;
};


// Defines the "Battery Sensor"
class MayflyOnboardBatt : public virtual MayflyOnboardSensors
{
public:
    MayflyOnboardBatt(int batteryPin);

    String getVarName(void) override;
    String getVarUnit(void) override;
    float getValue(void) override;
    String getDreamHost(void) override;
};

#endif