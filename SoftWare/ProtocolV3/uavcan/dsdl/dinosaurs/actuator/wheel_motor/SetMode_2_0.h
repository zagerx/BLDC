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
// Source file:   /home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/SetMode.2.0.dsdl
// Generated at:  2025-03-04 13:41:26.568447 UTC
// Is deprecated: no
// Fixed port-ID: None
// Full name:     custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.SetMode
// Version:       2.0
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

#ifndef CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_SET_MODE_2_0_INCLUDED_
#define CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_SET_MODE_2_0_INCLUDED_

#include <nunavut/support/serialization.h>
#include <stdint.h>
#include <stdlib.h>
#include <uavcan/si/unit/acceleration/Scalar_1_0.h>
#include <uavcan/si/unit/velocity/Scalar_1_0.h>

static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_TARGET_ENDIANNESS == 1693710260,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/SetMode.2.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_OMIT_FLOAT_SERIALIZATION_SUPPORT == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/SetMode.2.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_SERIALIZATION_ASSERTS == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/SetMode.2.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_OVERRIDE_VARIABLE_ARRAY_CAPACITY == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/SetMode.2.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_CAST_FORMAT == 2368206204,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/SetMode.2.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );

#ifdef __cplusplus
extern "C" {
#endif

/// This type does not have a fixed port-ID. See https://forum.opencyphal.org/t/choosing-message-and-service-ids/889
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_2_0_HAS_FIXED_PORT_ID_ false

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.SetMode.2.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_2_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.SetMode"
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_2_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.SetMode.2.0"

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.SetMode.Request.2.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.SetMode.Request"
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.SetMode.Request.2.0"

/// Extent is the minimum amount of memory required to hold any serialized representation of any compatible
/// version of the data type; or, on other words, it is the the maximum possible size of received objects of this type.
/// The size is specified in bytes (rather than bits) because by definition, extent is an integer number of bytes long.
/// When allocating a deserialization (RX) buffer for this data type, it should be at least extent bytes large.
/// When allocating a serialization (TX) buffer, it is safe to use the size of the largest serialized representation
/// instead of the extent because it provides a tighter bound of the object size; it is safe because the concrete type
/// is always known during serialization (unlike deserialization). If not sure, use extent everywhere.
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_EXTENT_BYTES_                    13UL
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 13UL
static_assert(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_EXTENT_BYTES_ >= custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_SERIALIZATION_BUFFER_SIZE_BYTES_,
              "Internal constraint violation");

/// saturated uint8 DYNAMIC_MODE = 1
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_DYNAMIC_MODE (1U)

/// saturated uint8 CURRENT_MODE = 2
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_CURRENT_MODE (2U)

/// saturated uint8 SPEED_MODE = 3
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_SPEED_MODE (3U)

/// saturated uint8 POSITION_MODE = 4
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_POSITION_MODE (4U)

typedef struct
{
    /// saturated uint8 mode
    uint8_t mode;

    /// uavcan.si.unit.velocity.Scalar.1.0 max_velocity
    uavcan_si_unit_velocity_Scalar_1_0 max_velocity;

    /// uavcan.si.unit.acceleration.Scalar.1.0 acceleration
    uavcan_si_unit_acceleration_Scalar_1_0 acceleration;

    /// uavcan.si.unit.acceleration.Scalar.1.0 deceleration
    uavcan_si_unit_acceleration_Scalar_1_0 deceleration;
} custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0;

/// Serialize an instance into the provided buffer.
/// The lifetime of the resulting serialized representation is independent of the original instance.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original object where possible.
///
/// @param obj      The object to serialize.
///
/// @param buffer   The destination buffer. There are no alignment requirements.
///                 @see custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_SERIALIZATION_BUFFER_SIZE_BYTES_
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the buffer in bytes.
///                                 Upon return this value will be updated with the size of the constructed serialized
///                                 representation (in bytes); this value is then to be passed over to the transport
///                                 layer. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_serialize_(
    const custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0* const obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes)
{
    if ((obj == NULL) || (buffer == NULL) || (inout_buffer_size_bytes == NULL))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    const size_t capacity_bytes = *inout_buffer_size_bytes;
    if ((8U * (size_t) capacity_bytes) < 104UL)
    {
        return -NUNAVUT_ERROR_SERIALIZATION_BUFFER_TOO_SMALL;
    }
    // Notice that fields that are not an integer number of bytes long may overrun the space allocated for them
    // in the serialization buffer up to the next byte boundary. This is by design and is guaranteed to be safe.
    size_t offset_bits = 0U;
    {   // saturated uint8 mode
        // Saturation code not emitted -- native representation matches the serialized representation.
        buffer[offset_bits / 8U] = (uint8_t)(obj->mode);  // C std, 6.3.1.3 Signed and unsigned integers
        offset_bits += 8U;
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad0_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err0_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad0_);  // Optimize?
        if (_err0_ < 0)
        {
            return _err0_;
        }
        offset_bits += _pad0_;
    }
    {   // uavcan.si.unit.velocity.Scalar.1.0 max_velocity
        size_t _size_bytes0_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err1_ = uavcan_si_unit_velocity_Scalar_1_0_serialize_(
            &obj->max_velocity, &buffer[offset_bits / 8U], &_size_bytes0_);
        if (_err1_ < 0)
        {
            return _err1_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes0_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad1_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err2_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad1_);  // Optimize?
        if (_err2_ < 0)
        {
            return _err2_;
        }
        offset_bits += _pad1_;
    }
    {   // uavcan.si.unit.acceleration.Scalar.1.0 acceleration
        size_t _size_bytes1_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err3_ = uavcan_si_unit_acceleration_Scalar_1_0_serialize_(
            &obj->acceleration, &buffer[offset_bits / 8U], &_size_bytes1_);
        if (_err3_ < 0)
        {
            return _err3_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes1_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad2_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err4_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad2_);  // Optimize?
        if (_err4_ < 0)
        {
            return _err4_;
        }
        offset_bits += _pad2_;
    }
    {   // uavcan.si.unit.acceleration.Scalar.1.0 deceleration
        size_t _size_bytes2_ = 4UL;  // Nested object (max) size, in bytes.
        int8_t _err5_ = uavcan_si_unit_acceleration_Scalar_1_0_serialize_(
            &obj->deceleration, &buffer[offset_bits / 8U], &_size_bytes2_);
        if (_err5_ < 0)
        {
            return _err5_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes2_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad3_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err6_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad3_);  // Optimize?
        if (_err6_ < 0)
        {
            return _err6_;
        }
        offset_bits += _pad3_;
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
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_deserialize_(
    custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0* const out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes)
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
    // uavcan.si.unit.velocity.Scalar.1.0 max_velocity
    {
        size_t _size_bytes3_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err7_ = uavcan_si_unit_velocity_Scalar_1_0_deserialize_(
            &out_obj->max_velocity, &buffer[offset_bits / 8U], &_size_bytes3_);
        if (_err7_ < 0)
        {
            return _err7_;
        }
        offset_bits += _size_bytes3_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.acceleration.Scalar.1.0 acceleration
    {
        size_t _size_bytes4_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err8_ = uavcan_si_unit_acceleration_Scalar_1_0_deserialize_(
            &out_obj->acceleration, &buffer[offset_bits / 8U], &_size_bytes4_);
        if (_err8_ < 0)
        {
            return _err8_;
        }
        offset_bits += _size_bytes4_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.si.unit.acceleration.Scalar.1.0 deceleration
    {
        size_t _size_bytes5_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err9_ = uavcan_si_unit_acceleration_Scalar_1_0_deserialize_(
            &out_obj->deceleration, &buffer[offset_bits / 8U], &_size_bytes5_);
        if (_err9_ < 0)
        {
            return _err9_;
        }
        offset_bits += _size_bytes5_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    *inout_buffer_size_bytes = (size_t) (nunavutChooseMin(offset_bits, capacity_bits) / 8U);
    return NUNAVUT_SUCCESS;
}

/// Initialize an instance to default values. Does nothing if @param out_obj is NULL.
/// This function intentionally leaves inactive elements uninitialized; for example, members of a variable-length
/// array beyond its length are left uninitialized; aliased union memory that is not used by the first union field
/// is left uninitialized, etc. If full zero-initialization is desired, just use memset(&obj, 0, sizeof(obj)).
static inline void custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_initialize_(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0* const out_obj)
{
    if (out_obj != NULL)
    {
        size_t size_bytes = 0;
        const uint8_t buf = 0;
        const int8_t err = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Request_2_0_deserialize_(out_obj, &buf, &size_bytes);

        (void) err;
    }
}

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.SetMode.Response.2.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.SetMode.Response"
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.SetMode.Response.2.0"

/// Extent is the minimum amount of memory required to hold any serialized representation of any compatible
/// version of the data type; or, on other words, it is the the maximum possible size of received objects of this type.
/// The size is specified in bytes (rather than bits) because by definition, extent is an integer number of bytes long.
/// When allocating a deserialization (RX) buffer for this data type, it should be at least extent bytes large.
/// When allocating a serialization (TX) buffer, it is safe to use the size of the largest serialized representation
/// instead of the extent because it provides a tighter bound of the object size; it is safe because the concrete type
/// is always known during serialization (unlike deserialization). If not sure, use extent everywhere.
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_EXTENT_BYTES_                    4UL
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 4UL
static_assert(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_EXTENT_BYTES_ >= custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_SERIALIZATION_BUFFER_SIZE_BYTES_,
              "Internal constraint violation");

/// saturated int32 SET_SUCCESS = 0
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_SET_SUCCESS (0L)

/// saturated int32 FAILED = 1
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_FAILED (1L)

/// saturated int32 PARAMETER_NOT_INIT = 2
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_PARAMETER_NOT_INIT (2L)

typedef struct
{
    /// saturated int32 status
    int32_t status;
} custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0;

/// Serialize an instance into the provided buffer.
/// The lifetime of the resulting serialized representation is independent of the original instance.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original object where possible.
///
/// @param obj      The object to serialize.
///
/// @param buffer   The destination buffer. There are no alignment requirements.
///                 @see custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_SERIALIZATION_BUFFER_SIZE_BYTES_
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the buffer in bytes.
///                                 Upon return this value will be updated with the size of the constructed serialized
///                                 representation (in bytes); this value is then to be passed over to the transport
///                                 layer. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_serialize_(
    const custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0* const obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes)
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
        const int8_t _err10_ = nunavutSetIxx(&buffer[0], capacity_bytes, offset_bits, obj->status, 32U);
        if (_err10_ < 0)
        {
            return _err10_;
        }
        offset_bits += 32U;
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad4_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err11_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad4_);  // Optimize?
        if (_err11_ < 0)
        {
            return _err11_;
        }
        offset_bits += _pad4_;
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
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_deserialize_(
    custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0* const out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes)
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
static inline void custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_initialize_(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0* const out_obj)
{
    if (out_obj != NULL)
    {
        size_t size_bytes = 0;
        const uint8_t buf = 0;
        const int8_t err = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetMode_Response_2_0_deserialize_(out_obj, &buf, &size_bytes);

        (void) err;
    }
}

#ifdef __cplusplus
}
#endif
#endif // CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_SET_MODE_2_0_INCLUDED_
