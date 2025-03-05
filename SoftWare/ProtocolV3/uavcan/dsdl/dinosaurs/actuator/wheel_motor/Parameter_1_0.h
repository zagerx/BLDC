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
// Source file:   /home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Parameter.1.0.dsdl
// Generated at:  2025-03-04 13:41:26.537523 UTC
// Is deprecated: no
// Fixed port-ID: None
// Full name:     custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Parameter
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

#ifndef CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_PARAMETER_1_0_INCLUDED_
#define CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_PARAMETER_1_0_INCLUDED_

#include <nunavut/support/serialization.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <uavcan/si/unit/acceleration/Scalar_1_0.h>
#include <uavcan/si/unit/duration/Scalar_1_0.h>
#include <uavcan/si/unit/electric_current/Scalar_1_0.h>
#include <uavcan/si/unit/length/Scalar_1_0.h>
#include <uavcan/si/unit/velocity/Scalar_1_0.h>

static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_TARGET_ENDIANNESS == 1693710260,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Parameter.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_OMIT_FLOAT_SERIALIZATION_SUPPORT == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Parameter.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_SERIALIZATION_ASSERTS == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Parameter.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_OVERRIDE_VARIABLE_ARRAY_CAPACITY == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Parameter.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_CAST_FORMAT == 2368206204,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/Parameter.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );

#ifdef __cplusplus
extern "C" {
#endif

/// This type does not have a fixed port-ID. See https://forum.opencyphal.org/t/choosing-message-and-service-ids/889
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_1_0_HAS_FIXED_PORT_ID_ false

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Parameter.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Parameter"
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Parameter.1.0"

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Parameter.Request.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Parameter.Request"
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Parameter.Request.1.0"

/// Extent is the minimum amount of memory required to hold any serialized representation of any compatible
/// version of the data type; or, on other words, it is the the maximum possible size of received objects of this type.
/// The size is specified in bytes (rather than bits) because by definition, extent is an integer number of bytes long.
/// When allocating a deserialization (RX) buffer for this data type, it should be at least extent bytes large.
/// When allocating a serialization (TX) buffer, it is safe to use the size of the largest serialized representation
/// instead of the extent because it provides a tighter bound of the object size; it is safe because the concrete type
/// is always known during serialization (unlike deserialization). If not sure, use extent everywhere.
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_EXTENT_BYTES_                    256UL
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 152UL
static_assert(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_EXTENT_BYTES_ >= custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_,
              "Internal constraint violation");

/// Array metadata for: saturated bool[<=16] odometry_polarity
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_odometry_polarity_ARRAY_CAPACITY_           16U
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_odometry_polarity_ARRAY_IS_VARIABLE_LENGTH_ true

/// Array metadata for: saturated bool[<=16] motor_polarity
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_motor_polarity_ARRAY_CAPACITY_           16U
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_motor_polarity_ARRAY_IS_VARIABLE_LENGTH_ true

/// Array metadata for: uavcan.si.unit.length.Scalar.1.0[<=16] radius
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_radius_ARRAY_CAPACITY_           16U
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_radius_ARRAY_IS_VARIABLE_LENGTH_ true

typedef struct
{
    /// saturated float32 current_kp
    float current_kp;

    /// saturated float32 current_ki
    float current_ki;

    /// saturated float32 current_kd
    float current_kd;

    /// saturated float32 position_kp
    float position_kp;

    /// saturated float32 position_ki
    float position_ki;

    /// saturated float32 position_kd
    float position_kd;

    /// saturated float32 voltage_kp
    float voltage_kp;

    /// saturated float32 voltage_ki
    float voltage_ki;

    /// saturated float32 voltage_kd
    float voltage_kd;

    /// saturated float32 velocity_kp
    float velocity_kp;

    /// saturated float32 velocity_ki
    float velocity_ki;

    /// saturated float32 velocity_kd
    float velocity_kd;

    /// saturated uint16 overcurrent_trigger_time_ms
    uint16_t overcurrent_trigger_time_ms;

    /// saturated uint16 overload_trigger_time_ms
    uint16_t overload_trigger_time_ms;

    /// saturated uint16 overvoltage_trigger_time_ms
    uint16_t overvoltage_trigger_time_ms;

    /// saturated uint16 communicate_timeout_trigger_time_ms
    uint16_t communicate_timeout_trigger_time_ms;

    /// uavcan.si.unit.duration.Scalar.1.0 data_update_duration
    uavcan_si_unit_duration_Scalar_1_0 data_update_duration;

    /// uavcan.si.unit.velocity.Scalar.1.0 max_velocity
    uavcan_si_unit_velocity_Scalar_1_0 max_velocity;

    /// uavcan.si.unit.velocity.Scalar.1.0 max_velocity_positon_mode
    uavcan_si_unit_velocity_Scalar_1_0 max_velocity_positon_mode;

    /// uavcan.si.unit.acceleration.Scalar.1.0 acceleration
    uavcan_si_unit_acceleration_Scalar_1_0 acceleration;

    /// uavcan.si.unit.acceleration.Scalar.1.0 deceleration
    uavcan_si_unit_acceleration_Scalar_1_0 deceleration;

    /// saturated bool[<=16] odometry_polarity
    struct  /// Array address equivalence guarantee: &elements[0] == &odometry_polarity
    {
        /// Bitpacked array, capacity 16 bits. Access via @ref nunavutSetBit(), @ref nunavutGetBit().
        uint8_t bitpacked[2];
        size_t count;
    } odometry_polarity;

    /// saturated bool[<=16] motor_polarity
    struct  /// Array address equivalence guarantee: &elements[0] == &motor_polarity
    {
        /// Bitpacked array, capacity 16 bits. Access via @ref nunavutSetBit(), @ref nunavutGetBit().
        uint8_t bitpacked[2];
        size_t count;
    } motor_polarity;

    /// saturated bool emergency_io_active
    bool emergency_io_active;

    /// uavcan.si.unit.electric_current.Scalar.1.0 overload_current
    uavcan_si_unit_electric_current_Scalar_1_0 overload_current;

    /// uavcan.si.unit.length.Scalar.1.0[<=16] radius
    struct  /// Array address equivalence guarantee: &elements[0] == &radius
    {
        uavcan_si_unit_length_Scalar_1_0 elements[custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_radius_ARRAY_CAPACITY_];
        size_t count;
    } radius;
} custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0;

/// Serialize an instance into the provided buffer.
/// The lifetime of the resulting serialized representation is independent of the original instance.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original object where possible.
///
/// @param obj      The object to serialize.
///
/// @param buffer   The destination buffer. There are no alignment requirements.
///                 @see custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the buffer in bytes.
///                                 Upon return this value will be updated with the size of the constructed serialized
///                                 representation (in bytes); this value is then to be passed over to the transport
///                                 layer. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_serialize_(
    const custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0* const obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes)
{
    if ((obj == NULL) || (buffer == NULL) || (inout_buffer_size_bytes == NULL))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    const size_t capacity_bytes = *inout_buffer_size_bytes;
    if ((8U * (size_t) capacity_bytes) < 1216UL)
    {
        return -NUNAVUT_ERROR_SERIALIZATION_BUFFER_TOO_SMALL;
    }
    // Notice that fields that are not an integer number of bytes long may overrun the space allocated for them
    // in the serialization buffer up to the next byte boundary. This is by design and is guaranteed to be safe.
    size_t offset_bits = 0U;
    {   // saturated float32 current_kp
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err0_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->current_kp);
        if (_err0_ < 0)
        {
            return _err0_;
        }
        offset_bits += 32U;
    }
    {   // saturated float32 current_ki
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err1_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->current_ki);
        if (_err1_ < 0)
        {
            return _err1_;
        }
        offset_bits += 32U;
    }
    {   // saturated float32 current_kd
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err2_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->current_kd);
        if (_err2_ < 0)
        {
            return _err2_;
        }
        offset_bits += 32U;
    }
    {   // saturated float32 position_kp
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err3_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->position_kp);
        if (_err3_ < 0)
        {
            return _err3_;
        }
        offset_bits += 32U;
    }
    {   // saturated float32 position_ki
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err4_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->position_ki);
        if (_err4_ < 0)
        {
            return _err4_;
        }
        offset_bits += 32U;
    }
    {   // saturated float32 position_kd
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err5_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->position_kd);
        if (_err5_ < 0)
        {
            return _err5_;
        }
        offset_bits += 32U;
    }
    {   // saturated float32 voltage_kp
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err6_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->voltage_kp);
        if (_err6_ < 0)
        {
            return _err6_;
        }
        offset_bits += 32U;
    }
    {   // saturated float32 voltage_ki
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err7_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->voltage_ki);
        if (_err7_ < 0)
        {
            return _err7_;
        }
        offset_bits += 32U;
    }
    {   // saturated float32 voltage_kd
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err8_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->voltage_kd);
        if (_err8_ < 0)
        {
            return _err8_;
        }
        offset_bits += 32U;
    }
    {   // saturated float32 velocity_kp
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err9_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->velocity_kp);
        if (_err9_ < 0)
        {
            return _err9_;
        }
        offset_bits += 32U;
    }
    {   // saturated float32 velocity_ki
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err10_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->velocity_ki);
        if (_err10_ < 0)
        {
            return _err10_;
        }
        offset_bits += 32U;
    }
    {   // saturated float32 velocity_kd
        // Saturation code not emitted -- assume the native representation of float32 is conformant.
        static_assert(NUNAVUT_PLATFORM_IEEE754_FLOAT, "Native IEEE754 binary32 required. TODO: relax constraint");
        const int8_t _err11_ = nunavutSetF32(&buffer[0], capacity_bytes, offset_bits, obj->velocity_kd);
        if (_err11_ < 0)
        {
            return _err11_;
        }
        offset_bits += 32U;
    }
    {   // saturated uint16 overcurrent_trigger_time_ms
        // Saturation code not emitted -- native representation matches the serialized representation.
        const int8_t _err12_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, obj->overcurrent_trigger_time_ms, 16U);
        if (_err12_ < 0)
        {
            return _err12_;
        }
        offset_bits += 16U;
    }
    {   // saturated uint16 overload_trigger_time_ms
        // Saturation code not emitted -- native representation matches the serialized representation.
        const int8_t _err13_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, obj->overload_trigger_time_ms, 16U);
        if (_err13_ < 0)
        {
            return _err13_;
        }
        offset_bits += 16U;
    }
    {   // saturated uint16 overvoltage_trigger_time_ms
        // Saturation code not emitted -- native representation matches the serialized representation.
        const int8_t _err14_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, obj->overvoltage_trigger_time_ms, 16U);
        if (_err14_ < 0)
        {
            return _err14_;
        }
        offset_bits += 16U;
    }
    {   // saturated uint16 communicate_timeout_trigger_time_ms
        // Saturation code not emitted -- native representation matches the serialized representation.
        const int8_t _err15_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, obj->communicate_timeout_trigger_time_ms, 16U);
        if (_err15_ < 0)
        {
            return _err15_;
        }
        offset_bits += 16U;
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad0_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err16_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad0_);  // Optimize?
        if (_err16_ < 0)
        {
            return _err16_;
        }
        offset_bits += _pad0_;
    }
    {   // uavcan.si.unit.duration.Scalar.1.0 data_update_duration
        size_t _size_bytes0_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err17_ = uavcan_si_unit_duration_Scalar_1_0_serialize_(
            &obj->data_update_duration, &buffer[offset_bits / 8U], &_size_bytes0_);
        if (_err17_ < 0)
        {
            return _err17_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes0_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad1_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err18_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad1_);  // Optimize?
        if (_err18_ < 0)
        {
            return _err18_;
        }
        offset_bits += _pad1_;
    }
    {   // uavcan.si.unit.velocity.Scalar.1.0 max_velocity
        size_t _size_bytes1_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err19_ = uavcan_si_unit_velocity_Scalar_1_0_serialize_(
            &obj->max_velocity, &buffer[offset_bits / 8U], &_size_bytes1_);
        if (_err19_ < 0)
        {
            return _err19_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes1_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad2_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err20_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad2_);  // Optimize?
        if (_err20_ < 0)
        {
            return _err20_;
        }
        offset_bits += _pad2_;
    }
    {   // uavcan.si.unit.velocity.Scalar.1.0 max_velocity_positon_mode
        size_t _size_bytes2_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err21_ = uavcan_si_unit_velocity_Scalar_1_0_serialize_(
            &obj->max_velocity_positon_mode, &buffer[offset_bits / 8U], &_size_bytes2_);
        if (_err21_ < 0)
        {
            return _err21_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes2_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad3_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err22_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad3_);  // Optimize?
        if (_err22_ < 0)
        {
            return _err22_;
        }
        offset_bits += _pad3_;
    }
    {   // uavcan.si.unit.acceleration.Scalar.1.0 acceleration
        size_t _size_bytes3_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err23_ = uavcan_si_unit_acceleration_Scalar_1_0_serialize_(
            &obj->acceleration, &buffer[offset_bits / 8U], &_size_bytes3_);
        if (_err23_ < 0)
        {
            return _err23_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes3_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad4_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err24_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad4_);  // Optimize?
        if (_err24_ < 0)
        {
            return _err24_;
        }
        offset_bits += _pad4_;
    }
    {   // uavcan.si.unit.acceleration.Scalar.1.0 deceleration
        size_t _size_bytes4_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err25_ = uavcan_si_unit_acceleration_Scalar_1_0_serialize_(
            &obj->deceleration, &buffer[offset_bits / 8U], &_size_bytes4_);
        if (_err25_ < 0)
        {
            return _err25_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes4_ * 8U;  // Advance by the size of the nested object.
    }
    {   // saturated bool[<=16] odometry_polarity
        if (obj->odometry_polarity.count > 16)
        {
            return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
        }
        // Array length prefix: truncated uint8
        buffer[offset_bits / 8U] = (uint8_t)(obj->odometry_polarity.count);  // C std, 6.3.1.3 Signed and unsigned integers
        offset_bits += 8U;
        // Optimization prospect: this item is aligned at the byte boundary, so it is possible to use memmove().
        nunavutCopyBits(&buffer[0], offset_bits, obj->odometry_polarity.count, &obj->odometry_polarity.bitpacked[0], 0U);
        offset_bits += obj->odometry_polarity.count;
    }
    {   // saturated bool[<=16] motor_polarity
        if (obj->motor_polarity.count > 16)
        {
            return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
        }
        // Array length prefix: truncated uint8
        const int8_t _err26_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, obj->motor_polarity.count, 8U);
        if (_err26_ < 0)
        {
            return _err26_;
        }
        offset_bits += 8U;
        nunavutCopyBits(&buffer[0], offset_bits, obj->motor_polarity.count, &obj->motor_polarity.bitpacked[0], 0U);
        offset_bits += obj->motor_polarity.count;
    }
    {   // saturated bool emergency_io_active
        if (obj->emergency_io_active)
        {
            buffer[offset_bits / 8U] = (uint8_t)(buffer[offset_bits / 8U] | (1U << (offset_bits % 8U)));
        }
        else
        {
            buffer[offset_bits / 8U] = (uint8_t)(buffer[offset_bits / 8U] & ~(1U << (offset_bits % 8U)));
        }
        offset_bits += 1U;
    }
    {   // void7
        const int8_t _err27_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, 7U);  // Optimize?
        if (_err27_ < 0)
        {
            return _err27_;
        }
        offset_bits += 7UL;
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad5_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err28_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad5_);  // Optimize?
        if (_err28_ < 0)
        {
            return _err28_;
        }
        offset_bits += _pad5_;
    }
    {   // uavcan.si.unit.electric_current.Scalar.1.0 overload_current
        size_t _size_bytes5_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err29_ = uavcan_si_unit_electric_current_Scalar_1_0_serialize_(
            &obj->overload_current, &buffer[offset_bits / 8U], &_size_bytes5_);
        if (_err29_ < 0)
        {
            return _err29_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes5_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad6_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err30_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad6_);  // Optimize?
        if (_err30_ < 0)
        {
            return _err30_;
        }
        offset_bits += _pad6_;
    }
    {   // uavcan.si.unit.length.Scalar.1.0[<=16] radius
        if (obj->radius.count > 16)
        {
            return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
        }
        // Array length prefix: truncated uint8
        buffer[offset_bits / 8U] = (uint8_t)(obj->radius.count);  // C std, 6.3.1.3 Signed and unsigned integers
        offset_bits += 8U;
        for (size_t _index0_ = 0U; _index0_ < obj->radius.count; ++_index0_)
        {
            size_t _size_bytes6_ = 4UL;  // Nested object (max) size, in bytes.
            int8_t _err31_ = uavcan_si_unit_length_Scalar_1_0_serialize_(
                &obj->radius.elements[_index0_], &buffer[offset_bits / 8U], &_size_bytes6_);
            if (_err31_ < 0)
            {
                return _err31_;
            }
            // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
            offset_bits += _size_bytes6_ * 8U;  // Advance by the size of the nested object.
        }
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad7_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err32_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad7_);  // Optimize?
        if (_err32_ < 0)
        {
            return _err32_;
        }
        offset_bits += _pad7_;
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
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_deserialize_(
    custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0* const out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes)
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
    // saturated float32 current_kp
    out_obj->current_kp = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated float32 current_ki
    out_obj->current_ki = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated float32 current_kd
    out_obj->current_kd = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated float32 position_kp
    out_obj->position_kp = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated float32 position_ki
    out_obj->position_ki = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated float32 position_kd
    out_obj->position_kd = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated float32 voltage_kp
    out_obj->voltage_kp = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated float32 voltage_ki
    out_obj->voltage_ki = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated float32 voltage_kd
    out_obj->voltage_kd = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated float32 velocity_kp
    out_obj->velocity_kp = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated float32 velocity_ki
    out_obj->velocity_ki = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated float32 velocity_kd
    out_obj->velocity_kd = nunavutGetF32(&buffer[0], capacity_bytes, offset_bits);
    offset_bits += 32U;
    // saturated uint16 overcurrent_trigger_time_ms
    out_obj->overcurrent_trigger_time_ms = nunavutGetU16(&buffer[0], capacity_bytes, offset_bits, 16);
    offset_bits += 16U;
    // saturated uint16 overload_trigger_time_ms
    out_obj->overload_trigger_time_ms = nunavutGetU16(&buffer[0], capacity_bytes, offset_bits, 16);
    offset_bits += 16U;
    // saturated uint16 overvoltage_trigger_time_ms
    out_obj->overvoltage_trigger_time_ms = nunavutGetU16(&buffer[0], capacity_bytes, offset_bits, 16);
    offset_bits += 16U;
    // saturated uint16 communicate_timeout_trigger_time_ms
    out_obj->communicate_timeout_trigger_time_ms = nunavutGetU16(&buffer[0], capacity_bytes, offset_bits, 16);
    offset_bits += 16U;
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.duration.Scalar.1.0 data_update_duration
    {
        size_t _size_bytes7_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err33_ = uavcan_si_unit_duration_Scalar_1_0_deserialize_(
            &out_obj->data_update_duration, &buffer[offset_bits / 8U], &_size_bytes7_);
        if (_err33_ < 0)
        {
            return _err33_;
        }
        offset_bits += _size_bytes7_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.velocity.Scalar.1.0 max_velocity
    {
        size_t _size_bytes8_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err34_ = uavcan_si_unit_velocity_Scalar_1_0_deserialize_(
            &out_obj->max_velocity, &buffer[offset_bits / 8U], &_size_bytes8_);
        if (_err34_ < 0)
        {
            return _err34_;
        }
        offset_bits += _size_bytes8_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.velocity.Scalar.1.0 max_velocity_positon_mode
    {
        size_t _size_bytes9_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err35_ = uavcan_si_unit_velocity_Scalar_1_0_deserialize_(
            &out_obj->max_velocity_positon_mode, &buffer[offset_bits / 8U], &_size_bytes9_);
        if (_err35_ < 0)
        {
            return _err35_;
        }
        offset_bits += _size_bytes9_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.acceleration.Scalar.1.0 acceleration
    {
        size_t _size_bytes10_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err36_ = uavcan_si_unit_acceleration_Scalar_1_0_deserialize_(
            &out_obj->acceleration, &buffer[offset_bits / 8U], &_size_bytes10_);
        if (_err36_ < 0)
        {
            return _err36_;
        }
        offset_bits += _size_bytes10_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.acceleration.Scalar.1.0 deceleration
    {
        size_t _size_bytes11_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err37_ = uavcan_si_unit_acceleration_Scalar_1_0_deserialize_(
            &out_obj->deceleration, &buffer[offset_bits / 8U], &_size_bytes11_);
        if (_err37_ < 0)
        {
            return _err37_;
        }
        offset_bits += _size_bytes11_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    // saturated bool[<=16] odometry_polarity
    // Array length prefix: truncated uint8
    if ((offset_bits + 8U) <= capacity_bits)
    {
        out_obj->odometry_polarity.count = buffer[offset_bits / 8U] & 255U;
    }
    else
    {
        out_obj->odometry_polarity.count = 0U;
    }
    offset_bits += 8U;
    if (out_obj->odometry_polarity.count > 16U)
    {
        return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
    }
    nunavutGetBits(&out_obj->odometry_polarity.bitpacked[0], &buffer[0], capacity_bytes, offset_bits, out_obj->odometry_polarity.count);
    offset_bits += out_obj->odometry_polarity.count;
    // saturated bool[<=16] motor_polarity
    // Array length prefix: truncated uint8
    out_obj->motor_polarity.count = nunavutGetU8(&buffer[0], capacity_bytes, offset_bits, 8);
    offset_bits += 8U;
    if (out_obj->motor_polarity.count > 16U)
    {
        return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
    }
    nunavutGetBits(&out_obj->motor_polarity.bitpacked[0], &buffer[0], capacity_bytes, offset_bits, out_obj->motor_polarity.count);
    offset_bits += out_obj->motor_polarity.count;
    // saturated bool emergency_io_active
    if (offset_bits < capacity_bits)
    {
        out_obj->emergency_io_active = (buffer[offset_bits / 8U] & (1U << (offset_bits % 8U))) != 0U;
    }
    else
    {
        out_obj->emergency_io_active = false;
    }
    offset_bits += 1U;
    // void7
    offset_bits += 7;
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.electric_current.Scalar.1.0 overload_current
    {
        size_t _size_bytes12_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err38_ = uavcan_si_unit_electric_current_Scalar_1_0_deserialize_(
            &out_obj->overload_current, &buffer[offset_bits / 8U], &_size_bytes12_);
        if (_err38_ < 0)
        {
            return _err38_;
        }
        offset_bits += _size_bytes12_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.length.Scalar.1.0[<=16] radius
    // Array length prefix: truncated uint8
    if ((offset_bits + 8U) <= capacity_bits)
    {
        out_obj->radius.count = buffer[offset_bits / 8U] & 255U;
    }
    else
    {
        out_obj->radius.count = 0U;
    }
    offset_bits += 8U;
    if (out_obj->radius.count > 16U)
    {
        return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
    }
    for (size_t _index1_ = 0U; _index1_ < out_obj->radius.count; ++_index1_)
    {
        {
            size_t _size_bytes13_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
            const int8_t _err39_ = uavcan_si_unit_length_Scalar_1_0_deserialize_(
                &out_obj->radius.elements[_index1_], &buffer[offset_bits / 8U], &_size_bytes13_);
            if (_err39_ < 0)
            {
                return _err39_;
            }
            offset_bits += _size_bytes13_ * 8U;  // Advance by the size of the nested serialized representation.
        }
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    *inout_buffer_size_bytes = (size_t) (nunavutChooseMin(offset_bits, capacity_bits) / 8U);
    return NUNAVUT_SUCCESS;
}

/// Initialize an instance to default values. Does nothing if @param out_obj is NULL.
/// This function intentionally leaves inactive elements uninitialized; for example, members of a variable-length
/// array beyond its length are left uninitialized; aliased union memory that is not used by the first union field
/// is left uninitialized, etc. If full zero-initialization is desired, just use memset(&obj, 0, sizeof(obj)).
static inline void custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_initialize_(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0* const out_obj)
{
    if (out_obj != NULL)
    {
        size_t size_bytes = 0;
        const uint8_t buf = 0;
        const int8_t err = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Request_1_0_deserialize_(out_obj, &buf, &size_bytes);

        (void) err;
    }
}

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Parameter.Response.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Parameter.Response"
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.Parameter.Response.1.0"

/// Extent is the minimum amount of memory required to hold any serialized representation of any compatible
/// version of the data type; or, on other words, it is the the maximum possible size of received objects of this type.
/// The size is specified in bytes (rather than bits) because by definition, extent is an integer number of bytes long.
/// When allocating a deserialization (RX) buffer for this data type, it should be at least extent bytes large.
/// When allocating a serialization (TX) buffer, it is safe to use the size of the largest serialized representation
/// instead of the extent because it provides a tighter bound of the object size; it is safe because the concrete type
/// is always known during serialization (unlike deserialization). If not sure, use extent everywhere.
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_EXTENT_BYTES_                    4UL
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 4UL
static_assert(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_EXTENT_BYTES_ >= custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_,
              "Internal constraint violation");

/// saturated int32 SET_SUCCESS = 0
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_SET_SUCCESS (0L)

/// saturated int32 PARAMETERS_EXCEPTION = 1
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_PARAMETERS_EXCEPTION (1L)

typedef struct
{
    /// saturated int32 status
    int32_t status;
} custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0;

/// Serialize an instance into the provided buffer.
/// The lifetime of the resulting serialized representation is independent of the original instance.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original object where possible.
///
/// @param obj      The object to serialize.
///
/// @param buffer   The destination buffer. There are no alignment requirements.
///                 @see custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the buffer in bytes.
///                                 Upon return this value will be updated with the size of the constructed serialized
///                                 representation (in bytes); this value is then to be passed over to the transport
///                                 layer. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_serialize_(
    const custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0* const obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes)
{
    if ((obj == NULL) || (buffer == NULL) || (inout_buffer_size_bytes == NULL))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    const size_t capacity_bytes = *inout_buffer_size_bytes;
    if ((8U * (size_t) capacity_bytes) < 32UL)
    {
        return -NUNAVUT_ERROR_SERIALIZATION_BUFFER_TOO_SMALL;
    }
    // Notice that fields that are not an integer number of bytes long may overrun the space allocated for them
    // in the serialization buffer up to the next byte boundary. This is by design and is guaranteed to be safe.
    size_t offset_bits = 0U;
    {   // saturated int32 status
        // Saturation code not emitted -- native representation matches the serialized representation.
        const int8_t _err40_ = nunavutSetIxx(&buffer[0], capacity_bytes, offset_bits, obj->status, 32U);
        if (_err40_ < 0)
        {
            return _err40_;
        }
        offset_bits += 32U;
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad8_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err41_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad8_);  // Optimize?
        if (_err41_ < 0)
        {
            return _err41_;
        }
        offset_bits += _pad8_;
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
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_deserialize_(
    custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0* const out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes)
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
    // saturated int32 status
    out_obj->status = nunavutGetI32(&buffer[0], capacity_bytes, offset_bits, 32);
    offset_bits += 32U;
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    *inout_buffer_size_bytes = (size_t) (nunavutChooseMin(offset_bits, capacity_bits) / 8U);
    return NUNAVUT_SUCCESS;
}

/// Initialize an instance to default values. Does nothing if @param out_obj is NULL.
/// This function intentionally leaves inactive elements uninitialized; for example, members of a variable-length
/// array beyond its length are left uninitialized; aliased union memory that is not used by the first union field
/// is left uninitialized, etc. If full zero-initialization is desired, just use memset(&obj, 0, sizeof(obj)).
static inline void custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_initialize_(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0* const out_obj)
{
    if (out_obj != NULL)
    {
        size_t size_bytes = 0;
        const uint8_t buf = 0;
        const int8_t err = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Parameter_Response_1_0_deserialize_(out_obj, &buf, &size_bytes);

        (void) err;
    }
}

#ifdef __cplusplus
}
#endif
#endif // CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_PARAMETER_1_0_INCLUDED_
