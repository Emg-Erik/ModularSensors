/**
 * @file GroPointGPLP3.h
 * @copyright Stroud Water Research Center
 * Part of the EnviroDIY ModularSensors library for Arduino.
 * This library is published under the BSD-3 license.
 * @author Anthony Aufdenkampe <aaufdenkampe@limno.com>
 *
 * @brief Contains the GroPointGPLP3 sensor subclass and the variable
 * subclasses GroPointGPLP3_Moist and GroPointGPLP3_Temp
 *
 * These are for the GroPoint Profile GPLP-3 Three-Segment Soil Moisture
 * and Temperature Profiling Probe.
 *
 * This depends on the GroPointParent super class.
 *
 * Documentation for the Modbus Protocol commands and responses can be found
 * within the documentation in the GroPointModbus library at:
 * https://github.com/EnviroDIY/GroPointModbus
 *
 * More detailed information on each variable can be found in the documentation
 * for the individual sensor probes
 */
/* clang-format off */
/**
 * @defgroup sensor_gplp3 GroPoint Profile GPLP-3 Soil Moisture & Temperature
 * Profiling Probe. Classes for the GroPoint Profile GPLP-3 Soil Moisture &
 * Temperature Probe.
 *
 * @ingroup gropoint_group
 *
 * @tableofcontents
 * @m_footernavigation
 *
 * @section sensor_gplp3_datasheet Sensor Datasheet
 * - [GroPoint Profile User Manual](https://www.gropoint.com/s/2625-N-T-GroPoint-Profile-User-Manual-V113.pdf), including Modbus Instructions.
 * - [GroPoint Profile Technical Info](https://www.gropoint.com/s/GroPoint-Profile-Technical-Info.pdf)
 *
 * @section sensor_gplp3_ctor Sensor Constructor
 * {{ @ref GroPointGPLP3::GroPointGPLP3 }}
 *
 * ___
 * @section sensor_gplp3_examples Example Code
 * The GPLP-3 Probe is used in the @menulink{gro_point_gplp3} example.
 *
 * @menusnip{gro_point_gplp3}
 */
/* clang-format on */

// Header Guards
#ifndef SRC_SENSORS_GROPOINTGPLP3_H_
#define SRC_SENSORS_GROPOINTGPLP3_H_

// Included Dependencies
#include "sensors/GroPointParent.h"
#include "GroPointModbus.h"

/** @ingroup sensor_gplp3 */
/**@{*/

/**
 * @anchor sensor_gplp3_var_counts
 * @name Sensor Variable Counts
 * The number of variables that can be returned by GPLP3
 */
/**@{*/
/// @brief Sensor::_numReturnedValues; the GPLP3 can report 3 values.
#define GPLP3_NUM_VARIABLES 9
/// @brief Sensor::_incCalcValues; we don't calculate any additional values.
#define GPLP3_INC_CALC_VARIABLES 0
/**@}*/

/**
 * @anchor sensor_gplp3_timing
 * @name Sensor Timing
 * The sensor timing for a GroPoint Profile GPLP-3
 */
/**@{*/
/**
 * @brief Sensor::_warmUpTime_ms; time before sensor responds after power - 1.6
 * seconds (1600ms).
 *
 * This is the time for communication to begin.
 */
#define GPLP3_WARM_UP_TIME_MS 350
/**
 * @brief Sensor::_stabilizationTime_ms; the GPLP-3 is stable after 100 ms.
 *
 */
#define GPLP3_STABILIZATION_TIME_MS 100
/// @brief Sensor::_measurementTime_ms; the GPLP-3 takes ~200 ms to complete a
/// measurement.
#define GPLP3_MEASUREMENT_TIME_MS 200
/**@}*/

/**
 * @anchor sensor_gplp3_moist
 * @name Moisture
 * The volumetric soil moisture variable from a GroPoint Profile GPLP-3
 * - Range is 0% to 50% volumetric water content
 * - Accuracy is ± 1%
 *
 * {{ @ref GroPointGPLP3_Moist::GroPointGPLP3_Moist }}
 */
/**@{*/
/// @brief Decimals places in string representation; soil moisture should have 1
/// - resolution is 0.1 %.
#define GPLP3_MOIST_RESOLUTION 1
/// @brief Variable name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/variablename/);
#define GPLP3_MOIST_VAR_NAME "volumetricWaterContent"
/// @brief Variable unit name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/units/);
#define GPLP3_MOIST_UNIT_NAME "percent"
/// @brief Default variable short code; "GPLP3Moist"
#define GPLP3_MOIST_DEFAULT_CODE "GPLP3Moist"
/**@}*/

/**
 * @anchor sensor_gplp3_temp
 * @name Temperature
 * The temperature variable from a GroPoint Profile GPLP-3
 * - Range is -20°C to + 70°C
 * - Accuracy is ± 0.5°C
 *
 * {{ @ref GroPointGPLP3_Temp::GroPointGPLP3_Temp }}
 */
/**@{*/
/// @brief Decimals places in string representation; temperature should have 1 -
/// resolution is 0.1°C.
#define GPLP3_TEMP_RESOLUTION 1
/// @brief Variable name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/variablename/);
/// "temperature"
#define GPLP3_TEMP_VAR_NAME "temperature"
/// @brief Variable unit name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/units/);
/// "degreeCelsius" (°C)
#define GPLP3_TEMP_UNIT_NAME "degreeCelsius"
/// @brief Default variable short code; "GPLP3Temp"
#define GPLP3_TEMP_DEFAULT_CODE "GPLP3Temp"
/**@}*/

// ===== Helpers: map 0-based inputs to the parent’s 0-based slots =====
// Moist indices 0..2 -> 0..2
static inline uint8_t gplp3VarIndexMoist(uint8_t idx) {
  if (idx > 2) idx = 0;           // clamp to a valid index
  return idx;                     // VWC occupies slots 0..2
}
// Temp indices 0..5 -> 3..8
static inline uint8_t gplp3VarIndexTemp(uint8_t idx) {
  if (idx > 5) idx = 0;           // clamp to a valid index
  return (uint8_t)(3 + idx);      // temps follow VWC, slots 3..8
}
/* clang-format off */
/**
 * @brief The Sensor sub-class for the
 * [GroPoint Profile GPLP3 probe](@ref sensor_gplp3).
 *
 * @ingroup sensor_gplp3
 */
/* clang-format on */
class GroPointGPLP3 : public GroPointParent {
 public:
    // Constructors with overloads
    /**
     * @brief Construct a new GroPoint GPLP3 object.
     *
     * @param modbusAddress The modbus address of the sensor.
     * @param stream An Arduino data stream for modbus communication.  See
     * [notes](@ref page_arduino_streams) for more information on what streams
     * can be used.
     * @param powerPin The pin on the mcu controlling power to the GPLP-3.
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
    GroPointGPLP3(byte modbusAddress, Stream* stream, int8_t powerPin,
                  int8_t powerPin2 = -1, int8_t enablePin = -1,
                  uint8_t measurementsToAverage = 1)
        : GroPointParent(modbusAddress, stream, powerPin, powerPin2, enablePin,
                         measurementsToAverage, GPLP3, "GroPointGPLP3",
                         GPLP3_NUM_VARIABLES, GPLP3_WARM_UP_TIME_MS,
                         GPLP3_STABILIZATION_TIME_MS, GPLP3_MEASUREMENT_TIME_MS,
                         GPLP3_INC_CALC_VARIABLES) {}
    /**
     * @copydoc GroPointGPLP3::GroPointGPLP3
     */
    GroPointGPLP3(byte modbusAddress, Stream& stream, int8_t powerPin,
                  int8_t powerPin2 = -1, int8_t enablePin = -1,
                  uint8_t measurementsToAverage = 1)
        : GroPointParent(modbusAddress, stream, powerPin, powerPin2, enablePin,
                         measurementsToAverage, GPLP3, "GroPointGPLP3",
                         GPLP3_NUM_VARIABLES, GPLP3_WARM_UP_TIME_MS,
                         GPLP3_STABILIZATION_TIME_MS, GPLP3_MEASUREMENT_TIME_MS,
                         GPLP3_INC_CALC_VARIABLES) {}
    /**
     * @brief Destroy the GroPoint GPLP3 object
     */
    ~GroPointGPLP3() {}
};


/* clang-format off */
/**
 * @brief The Variable sub-class used for the
 * [soil moisture output](@ref sensor_gplp3_moist) from a
 * [GroPoint Profile GPLP3 probe](@ref sensor_gplp3).
 *
 * @ingroup sensor_gplp3
 */
/* clang-format on */
class GroPointGPLP3_Moist : public Variable {
 public:
    /**
     * @brief Construct a new GroPointGPLP3_Moist object.
     *
     * @param parentSense The parent GroPointGPLP3 providing the result
     * values.
     * @param sensorVarNum The position the variable result holds in the
     * variable result array.  The GroPoint GPLP3 can have up to 3 soil moisture
     * results. When creating the variable for soil moisture, you must specify
     * the output number from the sensor.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable; optional with the default value of an empty string.
     * @param varCode A short code to help identify the variable in files;
     * optional with a default value of "GPLP3Moist".
     */
     // ---- Moisture (0..2) ----
     explicit GroPointGPLP3_Moist(GroPointGPLP3* parentSense,
                                const uint8_t  idx0,
                                const char*    uuid = "",
                                const char*    varCode = GPLP3_MOIST_DEFAULT_CODE)
        : Variable(parentSense,
                   gplp3VarIndexMoist(idx0),
                   (uint8_t)GPLP3_MOIST_RESOLUTION,
                   GPLP3_MOIST_VAR_NAME,
                   GPLP3_MOIST_UNIT_NAME,
                   varCode, uuid) {}
     
    /**
     * @brief Construct a new GroPointGPLP3_Moist object.
     *
     * @param sensorVarNum The position the variable result holds in the
     * variable result array.  The GroPoint GPLP3 can have up to 3 soil moisture
     * results. When creating the variable for soil moisture, you must specify
     * the output number from the sensor.
     *
     * @note This must be tied with a parent GroPointGPLP3 before it can be
     * used.
     */
    GroPointGPLP3_Moist(const uint8_t idx0)
        : Variable(gplp3VarIndexMoist(idx0),
                   (uint8_t)GPLP3_MOIST_RESOLUTION,
                   GPLP3_MOIST_VAR_NAME,
                   GPLP3_MOIST_UNIT_NAME,
                   GPLP3_MOIST_DEFAULT_CODE) {}
    /**
     * @brief Destroy the GroPointGPLP3_Moist object - no action needed.
     */
    ~GroPointGPLP3_Moist() {}
};

/* clang-format off */
/**
 * @brief The Variable sub-class used for the
 * [temperature output](@ref sensor_gplp3_temp) from a
 * [GroPoint Profile GPLP3 probe](@ref sensor_gplp3).
 *
 * @ingroup sensor_gplp3
 */
/* clang-format on */
class GroPointGPLP3_Temp : public Variable {
 public:
    /**
     * @brief Construct a new GroPointGPLP3_Temp object.
     *
     * @param parentSense The parent GroPointGPLP3 providing the result
     * values.
     * @param sensorVarNum The position the variable result holds in the
     * variable result array.  The GroPoint GPLP3 can have up to 6 temperature
     * results. When creating the variable for temperature, you must specify the
     * output number from the sensor.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable; optional with the default value of an empty string.
     * @param varCode A short code to help identify the variable in files;
     * optional with a default value of "GPLP3Temp".
     */
    explicit GroPointGPLP3_Temp(GroPointGPLP3* parentSense,
                            const uint8_t  idx0,
                            const char*    uuid = "",
                            const char*    varCode = GPLP3_TEMP_DEFAULT_CODE)
        : Variable(parentSense,
                   gplp3VarIndexTemp(idx0),
                   (uint8_t)GPLP3_TEMP_RESOLUTION,
                   GPLP3_TEMP_VAR_NAME,
                   GPLP3_TEMP_UNIT_NAME,
                   varCode, uuid) {}

    /**
     * @brief Construct a new GroPointGPLP3_Temp object.
     *
     * @param sensorVarNum The position the variable result holds in the
     * variable result array.  The GroPoint GPLP3 can have up to 6 temperature
     * results. When creating the variable for temperature, you must specify the
     * output number from the sensor.
     *
     * @note This must be tied with a parent GroPointGPLP3 before it can be
     * used.
     */
    GroPointGPLP3_Temp(const uint8_t idx0)
        : Variable(gplp3VarIndexTemp(idx0),
                   (uint8_t)GPLP3_TEMP_RESOLUTION,
                   GPLP3_TEMP_VAR_NAME,
                   GPLP3_TEMP_UNIT_NAME,
                   GPLP3_TEMP_DEFAULT_CODE) {}
    /**
     * @brief Destroy the GroPointGPLP3_Temp object - no action needed.
     */
    ~GroPointGPLP3_Temp() {}
};

/**@}*/
#endif  // SRC_SENSORS_GROPOINTGPLP3_H_
