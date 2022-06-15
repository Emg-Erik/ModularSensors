/**
 * @file VaisalaGMP252.h
 * Developed to work with EnviroDIY ModularSensors library for Arduino
 * @author Erik Geibrink
 *
 * @brief Contains the VaisalaGMP252 sensor subclass and the variable
 * subclasses TODO VaisalaGMP252_Turbidity and VaisalaGMP252_Temp.
 *
 * These are for the Vaisala GMP252 sensor Using Modbus.
 *
 * This depends on the VaisalaParent super class.
 *
 * Documentation for the Modbus Protocol commands and responses can be found
 * within the documentation in the VaisalaModbus library at:
 * https://github.com/EnviroDIY/VaisalaModbus TODO fix link and info
 */
/* clang-format off */
/**
 * @defgroup sensor_gmp252 Vaisala GMP252 CO2 Sensor
 * Classes for the Vaisala GMP252 CO2 sensor.
 *
 * @ingroup vaisala_group
 *
 * @tableofcontents
 * @m_footernavigation
 *
 * @section sensor_gmp252_datasheet Sensor Datasheet
 * - [Manual](https://docs.vaisala.com/r/kpRtO94gxmNAcexTr5QsJg/x7j2wy~_PuEJ6ylcNcqB3A)
 * - [Modbus Instructions](https://docs.vaisala.com/r/M211897EN-D/en-US/GUID-8F93F5AB-3796-4872-9205-91D789C77EF2)
 *
 * @section sensor_gmp252_ctor Sensor Constructor
 * {{ @ref VaisalaGMP252::VaisalaGMP252 }}
 *
 * ___
 * @section sensor_gmp252_examples Example Code
 * The Vaisala GMP252 CO2 sensor is used in the @menulink{gmp252}
 * example. TODO fix example
 *
 * @menusnip{gmp252}
 */
/* clang-format on */

// Header Guards
#ifndef SRC_SENSORS_VAISALAGMP252_H_
#define SRC_SENSORS_VAISALAGMP252_H_

// Included Dependencies
#include "sensors/VaisalaParent.h"


// Sensor Specific Defines
/** @ingroup sensor_gmp252 */
/**@{*/

/// @brief Sensor::_numReturnedValues; the GMP252 can report 2 values.
#define GMP252_NUM_VARIABLES 2

/**
 * @anchor sensor_GMP252_timing
 * @name Sensor Timing
 * The sensor timing for a Vaisala GMP252
 */
/**@{*/
/**
 * @brief Sensor::_warmUpTime_ms; GMP252 warms up in 12000ms.
 *
 * 12 sec to start deliver values
 */
#define GMP252_WARM_UP_TIME_MS 12000
/**
 * @brief Sensor::_stabilizationTime_ms; GMP252 is stable after 120s.
 *
 * warmup+stabilization > 120 s for consecutive readings to give different
 * results
 */
#define GMP252_STABILIZATION_TIME_MS 120000
/**
 * @brief Sensor::_measurementTime_ms; GMP252 take ~4s (4000ms) to complete a
 * measurement. TODO Check measurment time.
 *
 * Could potentially be lower with a longer stabilization time; more testing
 * needed.
 */
#define GMP252_MEASUREMENT_TIME_MS 4000
/**@}*/

/**
 * @anchor sensor_gmp252_co2
 * @name CO2
 * The CO2 variable from a Vaisala GMP252
 * - Range is 0 to 30000 ppmCO2
 * - Accuracy is +-40ppm (0-3000 ppmCO2)
 * - Accuracy is +-2% (3000-10 000 ppmCO2)
 * - Accuracy is +-3.5% (Up to 30 000 ppmCO2)
 *
 * {{ @ref VaisalaGMP252_carbonDioxide::VaisalaGMP252_carbonDioxide}}
 */
/**@{*/
/// @brief Decimals places in string representation; Carbon dioxide should have 0 -
/// resolution is 1 ppm.
#define GMP252_CO2_RESOLUTION 0
/// @brief Sensor variable number; CO2 is stored in sensorValues[0].
#define GMP252_CO2_VAR_NUM 0
/// @brief Variable name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/variablename/);
/// "carbonDioxide"
#define GMP252_CO2_VAR_NAME "carbonDioxide"
/// @brief Variable unit name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/units/);
/// "partPerMillion" (ppm)
#define GMP252_CO2_UNIT_NAME "partPerMillion"
/// @brief Default variable short code; "GMP252carbonDioxid"
#define GMP252_CO2_DEFAULT_CODE "GMP252carbonDioxid"
/**@}*/

/**
 * @anchor sensor_gmp252_temp
 * @name Temperature
 * The temperature variable from a Vaisala GMP252
 * - Range is -40°C to + 60°C
 * - Accuracy is ± 1°C TODO check accuracy
 *
 * {{ @ref VaisalaGMP252_Temp::VaisalaGMP252_Temp }}
 */
/**@{*/
/// @brief Decimals places in string representation; temperature should have 0 -
/// resolution is 1°C. TODO check output
#define GMP252_TEMP_RESOLUTION 0
/// @brief Sensor variable number; temperature is stored in sensorValues[1].
#define GMP252_TEMP_VAR_NUM 1
/// @brief Variable name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/variablename/);
/// "temperature"
#define GMP252_TEMP_VAR_NAME "temperature"
/// @brief Variable unit name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/units/);
/// "degreeCelsius" (°C)
#define GMP252_TEMP_UNIT_NAME "degreeCelsius"
/// @brief Default variable short code; "GMP252Temp"
#define GMP252_TEMP_DEFAULT_CODE "GMP252Temp"
/**@}*/


/* clang-format off */
/**
 * @brief The Sensor sub-class for the
 * [Vaisala GMP252-A CO2 sensor](@ref sensor_gmp252).
 *
 * @ingroup sensor_gmp252
 */
/* clang-format on */
class VaisalaGMP252 : public VaisalaParent {
 public:
    // Constructors with overloads
    /**
     * @brief Construct a new Vaisala GMP252 object.
     *
     * @param modbusAddress The modbus address of the sensor.
     * @param stream An Arduino data stream for modbus communication.  See
     * [notes](@ref page_arduino_streams) for more information on what streams
     * can be used.
     * @param powerPin The pin on the mcu controlling power to the GMP252.
     * Use -1 if it is continuously powered.
     * @param powerPin2 The pin on the mcu controlling power to the RS485
     * adapter, if it is different from that used to power the sensor.  Use -1
     * or omit if not applicable.
     * @param enablePin The pin on the mcu controlling the direction enable on
     * the RS485 adapter, if necessary; use -1 or omit if not applicable.
     * @note An RS485 adapter with integrated flow control is strongly
     * recommended.
     * @param measurementsToAverage The number of measurements to take and
     * average before giving a "final" result from the sensor; optional with a
     * default value of 1.
     */
    VaisalaGMP252(byte modbusAddress, Stream* stream, int8_t powerPin,
                   int8_t powerPin2 = -1, int8_t enablePin = -1,
                   uint8_t measurementsToAverage = 1)
        : VaisalaParent(modbusAddress, stream, powerPin, powerPin2,
                           enablePin, measurementsToAverage, vaisalaModel model,
                           "VaisalaGMP252", GMP252_NUM_VARIABLES,
                           GMP252_WARM_UP_TIME_MS, GMP252_STABILIZATION_TIME_MS,
                           GMP252_MEASUREMENT_TIME_MS) {}
    /**
     * @copydoc VaisalaGMP252::VaisalaGMP252
     */
    VaisalaGMP252(byte modbusAddress, Stream& stream, int8_t powerPin,
                   int8_t powerPin2 = -1, int8_t enablePin = -1,
                   uint8_t measurementsToAverage = 1)
        : VaisalaParent(modbusAddress, stream, powerPin, powerPin2,
                           enablePin, measurementsToAverage, vaisalaModel model,
                           "VaisalaGMP252", GMP252_NUM_VARIABLES,
                           GMP252_WARM_UP_TIME_MS, GMP252_STABILIZATION_TIME_MS,
                           GMP252_MEASUREMENT_TIME_MS) {}
    /**
     * @brief Destroy the Vaisala GMP252 object
     */
    ~VaisalaGMP252() {}
};


/* clang-format off */
/**
 * @brief The Variable sub-class used for the
 * [CO2 output](@ref sensor_gmp252_co2) from a
 * [Vaisala GMP252-A CO2 sensor](@ref sensor_gmp252).
 *
 * @ingroup sensor_gmp252
 */
/* clang-format on */
class VaisalaGMP252_carbonDioxide : public Variable {
 public:
    /**
     * @brief Construct a new VaisalaGMP252_carbonDioxide object.
     *
     * @param parentSense The parent VaisalaGMP252 providing the result
     * values.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable; optional with the default value of an empty string.
     * @param varCode A short code to help identify the variable in files;
     * optional with a default value of "GMP252carbonDioxide".
     */
    explicit VaisalaGMP252_carbonDioxide(
        VaisalaGMP252* parentSense, const char* uuid = "",
        const char* varCode = GMP252_CO2_DEFAULT_CODE)
        : Variable(parentSense, (const uint8_t)GMP252_CO2_VAR_NUM,
                   (uint8_t)GMP252_CO2_RESOLUTION, GMP252_CO2_VAR_NAME,
                   GMP252_CO2_UNIT_NAME, varCode, uuid) {}
    /**
     * @brief Construct a new VaisalaGMP252_carbonDioxide object.
     *
     * @note This must be tied with a parent VaisalaGMP252 before it can be
     * used.
     */
    VaisalaGMP252_carbonDioxide()
        : Variable((const uint8_t)GMP252_CO2_VAR_NUM,
                   (uint8_t)GMP252_CO2_RESOLUTION, GMP252_CO2_VAR_NAME,
                   GMP252_CO2_UNIT_NAME, GMP252_CO2_DEFAULT_CODE) {}
    /**
     * @brief Destroy the VaisalaGMP252_carbonDioxide object - no action needed.
     */
    ~VaisalaGMP252_carbonDioxide() {}
};


/* clang-format off */
/**
 * @brief The Variable sub-class used for the
 * [temperature output](@ref sensor_gmp252_temp) from a
 * [Vaisala GMP252-A CO2 sensor](@ref sensor_gmp252).
 *
 * @ingroup sensor_gmp252
 */
/* clang-format on */
class VaisalaGMP252_Temp : public Variable {
 public:
    /**
     * @brief Construct a new KellerNanolevel_Temp object.
     *
     * @param parentSense The parent KellerNanolevel providing the result
     * values.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable; optional with the default value of an empty string.
     * @param varCode A short code to help identify the variable in files;
     * optional with a default value of "kellerNanoTemp".
     */
    explicit VaisalaGMP252_Temp(
        VaisalaGMP252* parentSense, const char* uuid = "",
        const char* varCode = GMP252_TEMP_DEFAULT_CODE)
        : Variable(parentSense, (const uint8_t)GMP252_TEMP_VAR_NUM,
                   (uint8_t)GMP252_TEMP_RESOLUTION, GMP252_TEMP_VAR_NAME,
                   GMP252_TEMP_UNIT_NAME, varCode, uuid) {}
    /**
     * @brief Construct a new VaisalaGMP252_Temp object.
     *
     * @note This must be tied with a parent VaisalaGMP252 before it can be
     * used.
     */
    VaisalaGMP252_Temp()
        : Variable((const uint8_t)GMP252_TEMP_VAR_NUM,
                   (uint8_t)GMP252_TEMP_RESOLUTION, GMP252_TEMP_VAR_NAME,
                   GMP252_TEMP_UNIT_NAME, GMP252_TEMP_DEFAULT_CODE) {}
    /**
     * @brief Destroy the VaisalaGMP252_Temp object - no action needed.
     */
    ~VaisalaGMP252_Temp() {}
};

/**@}*/
#endif  // SRC_SENSORS_VAISALAGMP252_H_