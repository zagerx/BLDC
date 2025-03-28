// This is an AUTO-GENERATED Cyphal DSDL data type implementation. Curious? See https://opencyphal.org.
// You shouldn't attempt to edit this file.
//
// Checking this file under version control is not recommended unless it is used as part of a high-SIL
// safety-critical codebase. The typical usage scenario is to generate it as part of the build process.
//
// To avoid conflicts with definitions given in the source DSDL file, all entities created by the code generator
// are named with an underscore at the end, like foo_bar_().
//
// Generator:     nunavut-2.3.1 (serialization was enabled)
// Source file:   /home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Status.1.0.dsdl
// Generated at:  2025-03-04 13:41:26.619088 UTC
// Is deprecated: no
// Fixed port-ID: None
// Full name:     custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Status
// Version:       1.0
//
// Platform
//     python_implementation:  CPython
//     python_version:  3.10.12
//     python_release_level:  final
//     python_build:  ('main', 'Feb  4 2025 14:57:36')
//     python_compiler:  GCC 11.4.0
//     python_revision:
//     python_xoptions:  {}
//     runtime_platform:  Linux-6.8.0-52-generic-x86_64-with-glibc2.35
//
// Language Options
//     target_endianness:  any
//     omit_float_serialization_support:  False
//     enable_serialization_asserts:  False
//     enable_override_variable_array_capacity:  False
//     cast_format:  (({type}) {value})

#ifndef CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_STATUS_1_0_INCLUDED_
#define CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_STATUS_1_0_INCLUDED_

#include <nunavut/support/serialization.h>
#include <stdint.h>
#include <stdlib.h>
#include <uavcan/si/sample/temperature/Scalar_1_0.h>
#include <uavcan/si/unit/duration/Scalar_1_0.h>
#include <uavcan/si/unit/electric_current/Scalar_1_0.h>
#include <uavcan/si/unit/voltage/Scalar_1_0.h>
#include <uavcan/time/SynchronizedTimestamp_1_0.h>

static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_TARGET_ENDIANNESS == 1693710260,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Status.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_OMIT_FLOAT_SERIALIZATION_SUPPORT == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Status.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_SERIALIZATION_ASSERTS == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Status.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_OVERRIDE_VARIABLE_ARRAY_CAPACITY == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Status.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_CAST_FORMAT == 2368206204,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Status.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );

#ifdef __cplusplus
extern "C" {
#endif

/// This type does not have a fixed port-ID. See https://forum.opencyphal.org/t/choosing-message-and-service-ids/889
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_HAS_FIXED_PORT_ID_ false

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Status.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Status"
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Status.1.0"

/// Extent is the minimum amount of memory required to hold any serialized representation of any compatible
/// version of the data type; or, on other words, it is the the maximum possible size of received objects of this type.
/// The size is specified in bytes (rather than bits) because by definition, extent is an integer number of bytes long.
/// When allocating a deserialization (RX) buffer for this data type, it should be at least extent bytes large.
/// When allocating a serialization (TX) buffer, it is safe to use the size of the largest serialized representation
/// instead of the extent because it provides a tighter bound of the object size; it is safe because the concrete type
/// is always known during serialization (unlike deserialization). If not sure, use extent everywhere.
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_EXTENT_BYTES_                    273UL
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 273UL
static_assert(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_EXTENT_BYTES_ >= custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_,
              "Internal constraint violation");

/// Array metadata for: uavcan.si.unit.electric_current.Scalar.1.0[<=16] motor_current
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_motor_current_ARRAY_CAPACITY_           16U
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_motor_current_ARRAY_IS_VARIABLE_LENGTH_ true

/// Array metadata for: uavcan.si.sample.temperature.Scalar.1.0[<=16] motor_temperature
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_motor_temperature_ARRAY_CAPACITY_           16U
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_motor_temperature_ARRAY_IS_VARIABLE_LENGTH_ true

typedef struct
{
    /// uavcan.time.SynchronizedTimestamp.1.0 timestamp
    uavcan_time_SynchronizedTimestamp_1_0 timestamp;

    /// uavcan.si.unit.electric_current.Scalar.1.0 driver_input_current
    uavcan_si_unit_electric_current_Scalar_1_0 driver_input_current;

    /// uavcan.si.unit.electric_current.Scalar.1.0[<=16] motor_current
    struct  /// Array address equivalence guarantee: &elements[0] == &motor_current
    {
        uavcan_si_unit_electric_current_Scalar_1_0 elements[custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_motor_current_ARRAY_CAPACITY_];
        size_t count;
    } motor_current;

    /// uavcan.si.unit.voltage.Scalar.1.0 voltage
    uavcan_si_unit_voltage_Scalar_1_0 voltage;

    /// uavcan.si.sample.temperature.Scalar.1.0 driver_max_temperature
    uavcan_si_sample_temperature_Scalar_1_0 driver_max_temperature;

    /// uavcan.si.sample.temperature.Scalar.1.0[<=16] motor_temperature
    struct  /// Array address equivalence guarantee: &elements[0] == &motor_temperature
    {
        uavcan_si_sample_temperature_Scalar_1_0 elements[custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_motor_temperature_ARRAY_CAPACITY_];
        size_t count;
    } motor_temperature;

    /// uavcan.si.unit.duration.Scalar.1.0 data_update_duration
    uavcan_si_unit_duration_Scalar_1_0 data_update_duration;

    /// saturated uint8 mode
    uint8_t mode;
} custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0;

/// Serialize an instance into the provided buffer.
/// The lifetime of the resulting serialized representation is independent of the original instance.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original object where possible.
///
/// @param obj      The object to serialize.
///
/// @param buffer   The destination buffer. There are no alignment requirements.
///                 @see custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the buffer in bytes.
///                                 Upon return this value will be updated with the size of the constructed serialized
///                                 representation (in bytes); this value is then to be passed over to the transport
///                                 layer. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_serialize_(
    const custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0* const obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes)
{
    if ((obj == NULL) || (buffer == NULL) || (inout_buffer_size_bytes == NULL))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    const size_t capacity_bytes = *inout_buffer_size_bytes;
    if ((8U * (size_t) capacity_bytes) < 2184UL)
    {
        return -NUNAVUT_ERROR_SERIALIZATION_BUFFER_TOO_SMALL;
    }
    // Notice that fields that are not an integer number of bytes long may overrun the space allocated for them
    // in the serialization buffer up to the next byte boundary. This is by design and is guaranteed to be safe.
    size_t offset_bits = 0U;
    {   // uavcan.time.SynchronizedTimestamp.1.0 timestamp
        size_t _size_bytes0_ = 7UL;  // Nested object (max) size, in bytes.
        int8_t _err0_ = uavcan_time_SynchronizedTimestamp_1_0_serialize_(
            &obj->timestamp, &buffer[offset_bits / 8U], &_size_bytes0_);
        if (_err0_ < 0)
        {
            return _err0_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes0_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad0_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err1_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad0_);  // Optimize?
        if (_err1_ < 0)
        {
            return _err1_;
        }
        offset_bits += _pad0_;
    }
    {   // uavcan.si.unit.electric_current.Scalar.1.0 driver_input_current
        size_t _size_bytes1_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err2_ = uavcan_si_unit_electric_current_Scalar_1_0_serialize_(
            &obj->driver_input_current, &buffer[offset_bits / 8U], &_size_bytes1_);
        if (_err2_ < 0)
        {
            return _err2_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes1_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad1_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err3_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad1_);  // Optimize?
        if (_err3_ < 0)
        {
            return _err3_;
        }
        offset_bits += _pad1_;
    }
    {   // uavcan.si.unit.electric_current.Scalar.1.0[<=16] motor_current
        if (obj->motor_current.count > 16)
        {
            return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
        }
        // Array length prefix: truncated uint8
        buffer[offset_bits / 8U] = (uint8_t)(obj->motor_current.count);  // C std, 6.3.1.3 Signed and unsigned integers
        offset_bits += 8U;
        for (size_t _index0_ = 0U; _index0_ < obj->motor_current.count; ++_index0_)
        {
            size_t _size_bytes2_ = 4UL;  // Nested object (max) size, in bytes.
            int8_t _err4_ = uavcan_si_unit_electric_current_Scalar_1_0_serialize_(
                &obj->motor_current.elements[_index0_], &buffer[offset_bits / 8U], &_size_bytes2_);
            if (_err4_ < 0)
            {
                return _err4_;
            }
            // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
            offset_bits += _size_bytes2_ * 8U;  // Advance by the size of the nested object.
        }
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad2_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err5_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad2_);  // Optimize?
        if (_err5_ < 0)
        {
            return _err5_;
        }
        offset_bits += _pad2_;
    }
    {   // uavcan.si.unit.voltage.Scalar.1.0 voltage
        size_t _size_bytes3_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err6_ = uavcan_si_unit_voltage_Scalar_1_0_serialize_(
            &obj->voltage, &buffer[offset_bits / 8U], &_size_bytes3_);
        if (_err6_ < 0)
        {
            return _err6_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes3_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad3_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err7_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad3_);  // Optimize?
        if (_err7_ < 0)
        {
            return _err7_;
        }
        offset_bits += _pad3_;
    }
    {   // uavcan.si.sample.temperature.Scalar.1.0 driver_max_temperature
        size_t _size_bytes4_ = 11UL;  // Nested object (max) size, in bytes.
        int8_t _err8_ = uavcan_si_sample_temperature_Scalar_1_0_serialize_(
            &obj->driver_max_temperature, &buffer[offset_bits / 8U], &_size_bytes4_);
        if (_err8_ < 0)
        {
            return _err8_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes4_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad4_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err9_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad4_);  // Optimize?
        if (_err9_ < 0)
        {
            return _err9_;
        }
        offset_bits += _pad4_;
    }
    {   // uavcan.si.sample.temperature.Scalar.1.0[<=16] motor_temperature
        if (obj->motor_temperature.count > 16)
        {
            return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
        }
        // Array length prefix: truncated uint8
        buffer[offset_bits / 8U] = (uint8_t)(obj->motor_temperature.count);  // C std, 6.3.1.3 Signed and unsigned integers
        offset_bits += 8U;
        for (size_t _index1_ = 0U; _index1_ < obj->motor_temperature.count; ++_index1_)
        {
            size_t _size_bytes5_ = 11UL;  // Nested object (max) size, in bytes.
            int8_t _err10_ = uavcan_si_sample_temperature_Scalar_1_0_serialize_(
                &obj->motor_temperature.elements[_index1_], &buffer[offset_bits / 8U], &_size_bytes5_);
            if (_err10_ < 0)
            {
                return _err10_;
            }
            // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
            offset_bits += _size_bytes5_ * 8U;  // Advance by the size of the nested object.
        }
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad5_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err11_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad5_);  // Optimize?
        if (_err11_ < 0)
        {
            return _err11_;
        }
        offset_bits += _pad5_;
    }
    {   // uavcan.si.unit.duration.Scalar.1.0 data_update_duration
        size_t _size_bytes6_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err12_ = uavcan_si_unit_duration_Scalar_1_0_serialize_(
            &obj->data_update_duration, &buffer[offset_bits / 8U], &_size_bytes6_);
        if (_err12_ < 0)
        {
            return _err12_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes6_ * 8U;  // Advance by the size of the nested object.
    }
    {   // saturated uint8 mode
        // Saturation code not emitted -- native representation matches the serialized representation.
        buffer[offset_bits / 8U] = (uint8_t)(obj->mode);  // C std, 6.3.1.3 Signed and unsigned integers
        offset_bits += 8U;
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad6_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err13_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad6_);  // Optimize?
        if (_err13_ < 0)
        {
            return _err13_;
        }
        offset_bits += _pad6_;
    }
    // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
    *inout_buffer_size_bytes = (size_t) (offset_bits / 8U);
    return NUNAVUT_SUCCESS;
}

/// Deserialize an instance from the provided buffer.
/// The lifetime of the resulting object is independent of the original buffer.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original buffer where possible.
///
/// @param obj      The object to update from the provided serialized representation.
///
/// @param buffer   The source buffer containing the serialized representation. There are no alignment requirements.
///                 If the buffer is shorter or longer than expected, it will be implicitly zero-extended or truncated,
///                 respectively; see Specification for "implicit zero extension" and "implicit truncation" rules.
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the supplied serialized
///                                 representation, in bytes. Upon return this value will be updated with the
///                                 size of the consumed fragment of the serialized representation (in bytes),
///                                 which may be smaller due to the implicit truncation rule, but it is guaranteed
///                                 to never exceed the original buffer size even if the implicit zero extension rule
///                                 was activated. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_deserialize_(
    custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0* const out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes)
{
    if ((out_obj == NULL) || (inout_buffer_size_bytes == NULL) || ((buffer == NULL) && (0 != *inout_buffer_size_bytes)))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    if (buffer == NULL)
    {
        buffer = (const uint8_t*)"";
    }
    const size_t capacity_bytes = *inout_buffer_size_bytes;
    const size_t capacity_bits = capacity_bytes * (size_t) 8U;
    size_t offset_bits = 0U;
    // uavcan.time.SynchronizedTimestamp.1.0 timestamp
    {
        size_t _size_bytes7_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err14_ = uavcan_time_SynchronizedTimestamp_1_0_deserialize_(
            &out_obj->timestamp, &buffer[offset_bits / 8U], &_size_bytes7_);
        if (_err14_ < 0)
        {
            return _err14_;
        }
        offset_bits += _size_bytes7_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.electric_current.Scalar.1.0 driver_input_current
    {
        size_t _size_bytes8_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err15_ = uavcan_si_unit_electric_current_Scalar_1_0_deserialize_(
            &out_obj->driver_input_current, &buffer[offset_bits / 8U], &_size_bytes8_);
        if (_err15_ < 0)
        {
            return _err15_;
        }
        offset_bits += _size_bytes8_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.electric_current.Scalar.1.0[<=16] motor_current
    // Array length prefix: truncated uint8
    if ((offset_bits + 8U) <= capacity_bits)
    {
        out_obj->motor_current.count = buffer[offset_bits / 8U] & 255U;
    }
    else
    {
        out_obj->motor_current.count = 0U;
    }
    offset_bits += 8U;
    if (out_obj->motor_current.count > 16U)
    {
        return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
    }
    for (size_t _index2_ = 0U; _index2_ < out_obj->motor_current.count; ++_index2_)
    {
        {
            size_t _size_bytes9_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
            const int8_t _err16_ = uavcan_si_unit_electric_current_Scalar_1_0_deserialize_(
                &out_obj->motor_current.elements[_index2_], &buffer[offset_bits / 8U], &_size_bytes9_);
            if (_err16_ < 0)
            {
                return _err16_;
            }
            offset_bits += _size_bytes9_ * 8U;  // Advance by the size of the nested serialized representation.
        }
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.voltage.Scalar.1.0 voltage
    {
        size_t _size_bytes10_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err17_ = uavcan_si_unit_voltage_Scalar_1_0_deserialize_(
            &out_obj->voltage, &buffer[offset_bits / 8U], &_size_bytes10_);
        if (_err17_ < 0)
        {
            return _err17_;
        }
        offset_bits += _size_bytes10_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.sample.temperature.Scalar.1.0 driver_max_temperature
    {
        size_t _size_bytes11_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err18_ = uavcan_si_sample_temperature_Scalar_1_0_deserialize_(
            &out_obj->driver_max_temperature, &buffer[offset_bits / 8U], &_size_bytes11_);
        if (_err18_ < 0)
        {
            return _err18_;
        }
        offset_bits += _size_bytes11_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.sample.temperature.Scalar.1.0[<=16] motor_temperature
    // Array length prefix: truncated uint8
    if ((offset_bits + 8U) <= capacity_bits)
    {
        out_obj->motor_temperature.count = buffer[offset_bits / 8U] & 255U;
    }
    else
    {
        out_obj->motor_temperature.count = 0U;
    }
    offset_bits += 8U;
    if (out_obj->motor_temperature.count > 16U)
    {
        return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
    }
    for (size_t _index3_ = 0U; _index3_ < out_obj->motor_temperature.count; ++_index3_)
    {
        {
            size_t _size_bytes12_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
            const int8_t _err19_ = uavcan_si_sample_temperature_Scalar_1_0_deserialize_(
                &out_obj->motor_temperature.elements[_index3_], &buffer[offset_bits / 8U], &_size_bytes12_);
            if (_err19_ < 0)
            {
                return _err19_;
            }
            offset_bits += _size_bytes12_ * 8U;  // Advance by the size of the nested serialized representation.
        }
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.duration.Scalar.1.0 data_update_duration
    {
        size_t _size_bytes13_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err20_ = uavcan_si_unit_duration_Scalar_1_0_deserialize_(
            &out_obj->data_update_duration, &buffer[offset_bits / 8U], &_size_bytes13_);
        if (_err20_ < 0)
        {
            return _err20_;
        }
        offset_bits += _size_bytes13_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    // saturated uint8 mode
    if ((offset_bits + 8U) <= capacity_bits)
    {
        out_obj->mode = buffer[offset_bits / 8U] & 255U;
    }
    else
    {
        out_obj->mode = 0U;
    }
    offset_bits += 8U;
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    *inout_buffer_size_bytes = (size_t) (nunavutChooseMin(offset_bits, capacity_bits) / 8U);
    return NUNAVUT_SUCCESS;
}

/// Initialize an instance to default values. Does nothing if @param out_obj is NULL.
/// This function intentionally leaves inactive elements uninitialized; for example, members of a variable-length
/// array beyond its length are left uninitialized; aliased union memory that is not used by the first union field
/// is left uninitialized, etc. If full zero-initialization is desired, just use memset(&obj, 0, sizeof(obj)).
static inline void custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_initialize_(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0* const out_obj)
{
    if (out_obj != NULL)
    {
        size_t size_bytes = 0;
        const uint8_t buf = 0;
        const int8_t err = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Status_1_0_deserialize_(out_obj, &buf, &size_bytes);

        (void) err;
    }
}

#ifdef __cplusplus
}
#endif
#endif // CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_STATUS_1_0_INCLUDED_
