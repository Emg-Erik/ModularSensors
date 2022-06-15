/**
 * @file VaisalaParent.h
ö
 *
 * @brief Contains the VaisalaParent sensor subclass, itself a parent class
 * for all Vaisala sensors that communicate via Modbus and are supported by
 * the Vaisala library.
 *
 * Documentation for the Modbus Protocol commands and responses can be found
 * within the documentation in the VaisalaModbus library at:
 * https://github.com/EnviroDIY/VaisalaModbus TODO fix link and info
 */
/* clang-format off */
/**
 * @defgroup vaisala_group Vaisala Sensors
 * The Sensor and Variable objects for all Vaisala sensors.
 *
 * @ingroup the_sensors
 *
 *
 * This library currently supports the following [Vaisala](http://www.Vaisala.com/en/) sensors:
 * - [Vaisala GMP252-A CO2 sensor](https://www.vaisala.com/en/products/instruments-sensors-and-other-measurement-devices/instruments-industrial-measurements/gmp252)
 *     - [GMP252 Manual](https://docs.vaisala.com/r/kpRtO94gxmNAcexTr5QsJg/x7j2wy~_PuEJ6ylcNcqB3A)
 *     - [Modbus Instructions](https://docs.vaisala.com/r/kpRtO94gxmNAcexTr5QsJg/x7j2wy~_PuEJ6ylcNcqB3A)
 *     - [Class Documentation](@ref sensor_gmp252)

 *
 * All of these sensors require a 12-30V power supply and the power supply can be stopped between measurements.
 * They communicate via [Modbus RTU](https://en.wikipedia.org/wiki/Modbus) over [RS-485](https://en.wikipedia.org/wiki/RS-485).
 * To interface with them, you will need an RS485-to-TTL adapter.
 * The white wire of the Vaisala sensor will connect to the "B" pin of the adapter and the black wire will connect to "A".
 * The brown wire from the sensor should connect to the 12-30V power supply and the blue to ground.
 * The Vcc pin on the adapter should be connected to another power supply (voltage depends on the specific adapter) and the ground to the same ground.
 * The brown wire from the sensor _does not_ connect to the Vcc of the adapter.
 * The R/RO/RXD pin from the adapter connects to the TXD on the Arduino board and the D/DI/TXD pin from the adapter connects to the RXD.
 * If applicable, tie the RE and DE (receive/data enable) pins together and connect them to another pin on your board.
 * While this library supports an external enable pin, _we have had very bad luck with most of them and recommend against them_.
 * Adapters with automatic direction control tend to use very slightly more power, but have more stable communication.
 * There are a number of RS485-to-TTL adapters available.
 * When shopping for one, be mindful of the logic level of the TTL output by the adapter.
 * The MAX485, one of the most popular adapters, has a 5V logic level in the TTL signal.
 * This will _fry_ any board like the Mayfly that uses 3.3V logic.
 * You would need a voltage shifter in between the Mayfly and the MAX485 to make it work.
 *
 * The sensor constructor requires as input: the sensor modbus address, a stream instance for data (ie, ```Serial```), and one or two power pins.
 * The Arduino pin controlling the receive and data enable on your RS485-to-TTL adapter and the number of readings to average are optional.
 * (Use -1 for the second power pin and -1 for the enable pin if these don't apply and you want to average more than one reading.)
 * For all of these sensors Vaisala strongly recommends averaging 10 readings for each measurement.
 * Please see the section "[Notes on Arduino Streams and Software Serial](https://envirodiy.github.io/ModularSensors/page_arduino_streams.html)"
 * for more information about what streams can be used along with this library.
 * In tests on these sensors, SoftwareSerial_ExtInts _did not work_ to communicate with these sensors, because it isn't stable enough.
 * AltSoftSerial and HardwareSerial work fine.
 * Up to two power pins are provided so that the RS485 adapter, the sensor and/or an external power relay can be controlled separately.
 * If the power to everything is controlled by the same pin, use -1 for the second power pin or omit the argument.
 * If they are controlled by different pins _and no other sensors are dependent on power from either pin_ then the order of the pins doesn't matter.
 * If the RS485 adapter, sensor, or relay are controlled by different pins _and any other sensors are controlled by the same pins_ you should put the shared pin first and the un-shared pin second.
 * Both pins _cannot_ be shared pins.
 *
 * By default, this library cuts power to the sensors between readings.
 *
 * The lower level details of the communication with the sensors is managed by the
 * [EnviroDIY Vaisala library](https://github.com/EnviroDIY/VaisalaModbus) TODO add link and info
 */
/* clang-format on */

// Header Guards
#ifndef SRC_SENSORS_VAISALAPARENT_H_
#define SRC_SENSORS_VAISALAPARENT_H_

// Debugging Statement
// #define MS_VAISALAPARENT_DEBUG
// #define MS_VAISALAPARENT_DEBUG_DEEP

#ifdef MS_VAISALAPARENT_DEBUG
#define MS_DEBUGGING_STD "VaisalaParent"
#endif

#ifdef MS_VAISALAPARENT_DEBUG_DEEP
#define MS_DEBUGGING_DEEP "VaisalaParent"
#endif

// Included Dependencies
#include "ModSensorDebugger.h"
#undef MS_DEBUGGING_STD
#undef MS_DEBUGGING_DEEP
#include "VariableBase.h"
#include "SensorBase.h"
#include <VaisalaModbus.h>

// Sensor Specific Defines
/** @ingroup vaisala_group */
/**@{*/

/// @brief Sensor::_incCalcValues; we don't calculate any additional values.
#define VAISALA_INC_CALC_VARIABLES 0

/* clang-format off */
/**
 * @brief The Sensor sub-class for all the [Vaisala sensors](@ref vaisala_group)
 *
 * @ingroup vaisala_group
 */
/* clang-format on */
class VaisalaParent : public Sensor {
 public:
    /**
     * @brief Construct a new Vaisala Parent object.  This is only intended
     * to be used within this library.
     *
     * @param modbusAddress The modbus address of the sensor.
     * @param stream An Arduino data stream for modbus communication.  See
     * [notes](@ref page_arduino_streams) for more information on what streams
     * can be used.
     * @param powerPin The pin on the mcu controlling power to the Vaisala.
     * Use -1 if it is continuously powered.
     * @param powerPin2 The pin on the mcu controlling power to the RS485
     * adapter, if it is different from that used to power the sensor.  Use -1
     * or omit if not applicable.
     * @param enablePin The pin on the mcu controlling the direction enable on
     * the RS485 adapter, if necessary; use -1 or omit if not applicable.  An
     * RS485 adapter with integrated flow control is strongly recommended.
     * @param measurementsToAverage The number of measurements to take and
     * average before giving a "final" result from the sensor; optional with a
     * default value of 1.
     * @param model The model of Vaisala sensor. TODO Remove?
     * @param sensName The name of the sensor.  Defaults to "Vaisala-Sensor".
     * @param numVariables The number of variable results returned by the
     * sensor. Defaults to 2.
     * @param warmUpTime_ms The time in ms between when the sensor is powered on
     * and when it is ready to receive a wake command.  Defaults to 1500.
     * @param stabilizationTime_ms The time in ms between when the sensor
     * receives a wake command and when it is able to return stable values.
     * Defaults to 20,000 (20s).
     * @param measurementTime_ms The time in ms between when a measurement is
     * started and when the result value is available.  Defaults to 2000.
     */
    VaisalaParent(byte modbusAddress, Stream* stream, int8_t powerPin,
                     int8_t powerPin2, int8_t enablePin = -1,
                     uint8_t         measurementsToAverage = 1,
                     vaisalaModel model                 = 0,
                     const char*     sensName = "Vaisala-Sensor",
                     uint8_t numVariables = 2, uint32_t warmUpTime_ms = 1500,
                     uint32_t stabilizationTime_ms = 120000,
                     uint32_t measurementTime_ms   = 2000);
    /**
     * @copydoc VaisalaParent::VaisalaParent
     */
    VaisalaParent(byte modbusAddress, Stream& stream, int8_t powerPin,
                     int8_t powerPin2, int8_t enablePin = -1,
                     uint8_t         measurementsToAverage = 1,
                     vaisalaModel model                 = 0,
                     const char*     sensName = "Vaisala-Sensor",
                     uint8_t numVariables = 2, uint32_t warmUpTime_ms = 1500,
                     uint32_t stabilizationTime_ms = 120000,
                     uint32_t measurementTime_ms   = 2000);
    /**
     * @brief Destroy the Vaisala Parent object - no action taken
     */
    virtual ~VaisalaParent();

    /**
     * @copydoc Sensor::getSensorLocation()
     */
    String getSensorLocation(void) override;

    /**
     * @brief Do any one-time preparations needed before the sensor will be able
     * to take readings.
     *
     * This sets pin modes on the #_powerPin, adapter power, and adapter
     * enable pins.  It also sets the expected stream timeout for modbus and
     * updates the #_sensorStatus.   No sensor power is required.  This will
     * always return true.
     *
     * @return **bool** True if the setup was successful.
     */
    bool setup(void) override;
    /**
     * @brief Wake the sensor up, if necessary.  Do whatever it takes to get a
     * sensor in the proper state to begin a measurement.
     *
     * Verifies that the power is on and updates the #_sensorStatus.  This also
     * sets the #_millisSensorActivated timestamp.
     *
     * @note This does NOT include any wait for sensor readiness.
     *
     * @return **bool** True if the wake function completed successfully.
     */
    

    // Override these to use two power pins
    void powerUp(void) override;
    void powerDown(void) override;

    /**
     * @copydoc Sensor::addSingleMeasurementResult()
     */
    bool addSingleMeasurementResult(void) override;

 private:
    vaisala      _vsensor;
    vaisalaModel _model;
    byte         _modbusAddress;
    Stream*      _stream;
    int8_t       _RS485EnablePin;
    int8_t       _powerPin2;
};

#endif  // SRC_SENSORS_VAISALAPARENT_H_
