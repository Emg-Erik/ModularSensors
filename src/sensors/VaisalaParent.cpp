/**
 * @file VaisalaParent.cpp
 * Developed to work with EnviroDIY ModularSensors library for Arduino
 * @author Written By: Erik Geibrink

 * @brief Implements the Vaisala Parent class.
 */


#include "VaisalaParent.h"

// The constructor - need the sensor type, modbus address, power pin, stream for
// data, and number of readings to average
VaisalaParent::VaisalaParent(
    byte modbusAddress, Stream* stream, int8_t powerPin, int8_t powerPin2,
    int8_t enablePin, uint8_t measurementsToAverage, vaisalaModel model,
    const char* sensName, uint8_t numVariables, uint32_t warmUpTime_ms,
    uint32_t stabilizationTime_ms, uint32_t measurementTime_ms)
    : Sensor(sensName, numVariables, warmUpTime_ms, stabilizationTime_ms,
             measurementTime_ms, powerPin, -1, measurementsToAverage,
             VAISALA_INC_CALC_VARIABLES),
      _vsensor(), _model(model), _modbusAddress(modbusAddress), _stream(stream),
      _RS485EnablePin(enablePin), _powerPin2(powerPin2) {}
VaisalaParent::VaisalaParent(
    byte modbusAddress, Stream& stream, int8_t powerPin, int8_t powerPin2,
    int8_t enablePin, uint8_t measurementsToAverage, vaisalaModel model,
    const char* sensName, uint8_t numVariables, uint32_t warmUpTime_ms,
    uint32_t stabilizationTime_ms, uint32_t measurementTime_ms)
    : Sensor(sensName, numVariables, warmUpTime_ms, stabilizationTime_ms,
             measurementTime_ms, powerPin, -1, measurementsToAverage,
             VAISALA_INC_CALC_VARIABLES),
      _vsensor(), _model(model), _modbusAddress(modbusAddress),
      _stream(&stream), _RS485EnablePin(enablePin), _powerPin2(powerPin2) {}
// Destructor
VaisalaParent::~VaisalaParent() {}


// The sensor installation location on the Mayfly
String VaisalaParent::getSensorLocation(void) {
    String sensorLocation = F("modbus_0x");
    if (_modbusAddress < 16) sensorLocation += "0";
    sensorLocation += String(_modbusAddress, HEX);
    return sensorLocation;
}


bool VaisalaParent::setup(void) {
    bool retVal =
        Sensor::setup();  // this will set pin modes and the setup status bit
    if (_RS485EnablePin >= 0) pinMode(_RS485EnablePin, OUTPUT);
    if (_powerPin2 >= 0) pinMode(_powerPin2, OUTPUT);

#ifdef MS_VAISALAPARENT_DEBUG_DEEP
    _ysensor.setDebugStream(&DEEP_DEBUGGING_SERIAL_OUTPUT);
#endif

    // This sensor begin is just setting more pin modes, etc, no sensor power
    // required This realy can't fail so adding the return value is just for
    // show
    retVal &= _ysensor.begin(_model, _modbusAddress, _stream, _RS485EnablePin);

    return retVal;
}

// This turns on sensor power
void VaisalaParent::powerUp(void) {
    if (_powerPin >= 0) {
        MS_DBG(F("Powering"), getSensorNameAndLocation(), F("with pin"),
               _powerPin);
        digitalWrite(_powerPin, HIGH);
        // Mark the time that the sensor was powered
        _millisPowerOn = millis();
    }
    if (_powerPin2 >= 0) {
        MS_DBG(F("Applying secondary power to"), getSensorNameAndLocation(),
               F("with pin"), _powerPin2);
        digitalWrite(_powerPin2, HIGH);
    }
    if (_powerPin < 0 && _powerPin2 < 0) {
        MS_DBG(F("Power to"), getSensorNameAndLocation(),
               F("is not controlled by this library."));
    }
    // Set the status bit for sensor power attempt (bit 1) and success (bit 2)
    _sensorStatus |= 0b00000110;
}


// This turns off sensor power
void VaisalaParent::powerDown(void) {
    if (_powerPin >= 0) {
        MS_DBG(F("Turning off power to"), getSensorNameAndLocation(),
               F("with pin"), _powerPin);
        digitalWrite(_powerPin, LOW);
        // Unset the power-on time
        _millisPowerOn = 0;
        // Unset the activation time
        _millisSensorActivated = 0;
        // Unset the measurement request time
        _millisMeasurementRequested = 0;
        // Unset the status bits for sensor power (bits 1 & 2),
        // activation (bits 3 & 4), and measurement request (bits 5 & 6)
        _sensorStatus &= 0b10000001;
    }
    if (_powerPin2 >= 0) {
        MS_DBG(F("Turning off secondary power to"), getSensorNameAndLocation(),
               F("with pin"), _powerPin2);
        digitalWrite(_powerPin2, LOW);
    }
    if (_powerPin < 0 && _powerPin2 < 0) {
        MS_DBG(F("Power to"), getSensorNameAndLocation(),
               F("is not controlled by this library."));
        // Do NOT unset any status bits or timestamps if we didn't really power
        // down!
    }
}


bool VaisalaParent::addSingleMeasurementResult(void) {
    bool success = false;

    // Check a measurement was *successfully* started (status bit 6 set)
    // Only go on to get a result if it was
    if (bitRead(_sensorStatus, 6)) {
        
        // Initialize float variables
        int CO2Value_ppm  = -9999;
        float sensorTemperatureValueC  = -9999;

        // Get Values
        MS_DBG(F("Get Values from"), getSensorNameAndLocation());
        success = _vsensor.getValues(CO2Value_ppm, sensorTemperatureValueC);

        // Fix not-a-number values
        if (!success || isnan(CO2Value_ppm)) CO2Value_ppm = -9999;
        if (!success || isnan(sensorTemperatureValueC)) sensorTemperatureValueC = -9999;

        MS_DBG(F(" CO2-value_ppm:"), CO2Value_ppm);
        MS_DBG(F("  Temp_C:"), sensorTemperatureValueC);


        // Put values into the array
        verifyAndAddMeasurementResult(0, CO2Value_ppm);
        verifyAndAddMeasurementResult(1, sensorTemperatureValueC);            
        
    } else {
        MS_DBG(getSensorNameAndLocation(), F("is not currently measuring!"));
    }

    // Unset the time stamp for the beginning of this measurement
    _millisMeasurementRequested = 0;
    // Unset the status bits for a measurement request (bits 5 & 6)
    _sensorStatus &= 0b10011111;

    // Return true when finished
    return success;
}
