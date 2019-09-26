/*
 *SIMComSIM800.cpp
 *This file is part of the EnviroDIY modular sensors library for Arduino
 *
 *Initial library developement done by Sara Damiano (sdamiano@stroudcenter.org).
 *
 *This file is for the Adafruit Fona 2G, the Sodaq GPRSBee R4 and almost any
 * other module based on the SIMCOM SIM800 or SIM900
 *the SIMCOM SIM800h.
*/

// Included Dependencies
#include "SIMComSIM800.h"


// Constructor
SIMComSIM800::SIMComSIM800(Stream* modemStream,
                           int8_t powerPin, int8_t statusPin,
                           int8_t modemResetPin, int8_t modemSleepRqPin,
                           const char *apn)
  : loggerModem(powerPin, statusPin, HIGH,
                modemResetPin, modemSleepRqPin, false,
                SIM800_STATUS_TIME_MS, SIM800_DISCONNECT_TIME_MS,
                SIM800_WARM_UP_TIME_MS, SIM800_ATRESPONSE_TIME_MS),
    #ifdef MS_SIMCOMSIM800_DEBUG_DEEP
    _modemATDebugger(*modemStream, DEEP_DEBUGGING_SERIAL_OUTPUT),
    gsmModem(_modemATDebugger),
    #else
    gsmModem(*modemStream),
    #endif
    gsmClient(gsmModem)
{
    _apn = apn;
}


// Destructor
SIMComSIM800::~SIMComSIM800(){}

MS_MODEM_HARD_RESET(SIMComSIM800);
MS_MODEM_IS_INTERNET_AVAILABLE(SIMComSIM800);
MS_MODEM_GET_MODEM_SIGNAL_QUALITY(SIMComSIM800);
MS_MODEM_CONNECT_INTERNET(SIMComSIM800);

// NOTE:  Could actually get temperature from the Digi chip by entering command mode
float SIMComSIM800::getModemChipTemperature(void)
{
    MS_DBG(F("This modem doesn't return temperature!"));
    return (float)-9999;
}

// Create the wake and sleep methods for the modem
// These can be functions of any type and must return a boolean
bool SIMComSIM800::modemWakeFxn(void)
{
    // Must power on and then pulse on
    if (_modemSleepRqPin >= 0)
    {
        MS_DBG(F("Sending a wake-up pulse on pin"), _modemSleepRqPin, F("for SIM800"));
        digitalWrite(_modemSleepRqPin, LOW);
        delay(1100);  // >1s
        digitalWrite(_modemSleepRqPin, HIGH);
    }
    return true;
}


bool SIMComSIM800::modemSleepFxn(void)
{
    if (_modemSleepRqPin >= 0) // R410 must have access to PWRKEY pin to sleep
    {
        // Easiest to just go to sleep with the AT command rather than using pins
        MS_DBG(F("Asking SIM800 to power down"));
        return gsmModem.poweroff();
    }
    else  // DON'T go to sleep if we can't wake up!
    {
        return true;
    }
}


bool SIMComSIM800::extraModemSetup(void){}


void SIMComSIM800::modemPowerUp(void)
{
    if (_powerPin >= 0)
    {
        if (_modemSleepRqPin >= 0)
        {
            // The PWR_ON pin MUST be high at power up.
            digitalWrite(_modemSleepRqPin, HIGH);
        }
        MS_DBG(F("Powering"), getModemName(), F("with pin"), _powerPin);
        digitalWrite(_powerPin, HIGH);
        // Mark the time that the sensor was powered
        _millisPowerOn = millis();
    }
    else
    {
        MS_DBG(F("Power to"), getModemName(), F("is not controlled by this library."));
        // Mark the power-on time, just in case it had not been marked
        if (_millisPowerOn == 0) _millisPowerOn = millis();
    }
}
