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
// Source file:   /home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/control/MotionControl.1.0.dsdl
// Generated at:  2025-03-04 13:41:26.512565 UTC
// Is deprecated: no
// Fixed port-ID: None
// Full name:     custom_data_types_dsdl.dinosaurs.actuator.control.MotionControl
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

#ifndef CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_CONTROL_MOTION_CONTROL_1_0_INCLUDED_
#define CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_CONTROL_MOTION_CONTROL_1_0_INCLUDED_

#include <custom_data_types_dsdl/dinosaurs/actuator/control/DifferentialDriveMotionControl_1_0.h>
#include <nunavut/support/serialization.h>
#include <stdint.h>
#include <stdlib.h>

static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_TARGET_ENDIANNESS == 1693710260,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/control/MotionControl.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_OMIT_FLOAT_SERIALIZATION_SUPPORT == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/control/MotionControl.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_SERIALIZATION_ASSERTS == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/control/MotionControl.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_OVERRIDE_VARIABLE_ARRAY_CAPACITY == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/control/MotionControl.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_CAST_FORMAT == 2368206204,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/control/MotionControl.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );

#ifdef __cplusplus
extern "C" {
#endif

/// This type does not have a fixed port-ID. See https://forum.opencyphal.org/t/choosing-message-and-service-ids/889
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_1_0_HAS_FIXED_PORT_ID_ false

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.control.MotionControl.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.control.MotionControl"
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.control.MotionControl.1.0"

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.control.MotionControl.Request.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.control.MotionControl.Request"
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.control.MotionControl.Request.1.0"

/// Extent is the minimum amount of memory required to hold any serialized representation of any compatible
/// version of the data type; or, on other words, it is the the maximum possible size of received objects of this type.
/// The size is specified in bytes (rather than bits) because by definition, extent is an integer number of bytes long.
/// When allocating a deserialization (RX) buffer for this data type, it should be at least extent bytes large.
/// When allocating a serialization (TX) buffer, it is safe to use the size of the largest serialized representation
/// instead of the extent because it provides a tighter bound of the object size; it is safe because the concrete type
/// is always known during serialization (unlike deserialization). If not sure, use extent everywhere.
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_EXTENT_BYTES_                    128UL
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 9UL
static_assert(custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_EXTENT_BYTES_ >= custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_,
              "Internal constraint violation");

typedef struct
{
    union  /// The union is placed first to ensure that the active element address equals the struct address.
    {
        /// custom_data_types_dsdl.dinosaurs.actuator.control.DifferentialDriveMotionControl.1.0 ddmc
        custom_data_types_dsdl_dinosaurs_actuator_control_DifferentialDriveMotionControl_1_0 ddmc;

        /// saturated uint32 dummy
        uint32_t dummy;
    };
    uint8_t _tag_;
} custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0;

/// The number of fields in the union. Valid tag values range from zero to this value minus one, inclusive.
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_UNION_OPTION_COUNT_ 2U

/// Serialize an instance into the provided buffer.
/// The lifetime of the resulting serialized representation is independent of the original instance.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original object where possible.
///
/// @param obj      The object to serialize.
///
/// @param buffer   The destination buffer. There are no alignment requirements.
///                 @see custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the buffer in bytes.
///                                 Upon return this value will be updated with the size of the constructed serialized
///                                 representation (in bytes); this value is then to be passed over to the transport
///                                 layer. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_serialize_(
    const custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0* const obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes)
{
    if ((obj == NULL) || (buffer == NULL) || (inout_buffer_size_bytes == NULL))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    const size_t capacity_bytes = *inout_buffer_size_bytes;
    if ((8U * (size_t) capacity_bytes) < 72UL)
    {
        return -NUNAVUT_ERROR_SERIALIZATION_BUFFER_TOO_SMALL;
    }
    // Notice that fields that are not an integer number of bytes long may overrun the space allocated for them
    // in the serialization buffer up to the next byte boundary. This is by design and is guaranteed to be safe.
    size_t offset_bits = 0U;
    {   // Union tag field: truncated uint8
        buffer[offset_bits / 8U] = (uint8_t)(obj->_tag_);  // C std, 6.3.1.3 Signed and unsigned integers
        offset_bits += 8U;
    }
    if (0U == obj->_tag_)  // custom_data_types_dsdl.dinosaurs.actuator.control.DifferentialDriveMotionControl.1.0 ddmc
    {
        size_t _size_bytes0_ = 8UL;  // Nested object (max) size, in bytes.
        int8_t _err0_ = custom_data_types_dsdl_dinosaurs_actuator_control_DifferentialDriveMotionControl_1_0_serialize_(
            &obj->ddmc, &buffer[offset_bits / 8U], &_size_bytes0_);
        if (_err0_ < 0)
        {
            return _err0_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes0_ * 8U;  // Advance by the size of the nested object.
    }
    else if (1U == obj->_tag_)  // saturated uint32 dummy
    {
        // Saturation code not emitted -- native representation matches the serialized representation.
        const int8_t _err1_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, obj->dummy, 32U);
        if (_err1_ < 0)
        {
            return _err1_;
        }
        offset_bits += 32U;
    }
    else
    {
        return -NUNAVUT_ERROR_REPRESENTATION_BAD_UNION_TAG;
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad0_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err2_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad0_);  // Optimize?
        if (_err2_ < 0)
        {
            return _err2_;
        }
        offset_bits += _pad0_;
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
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_deserialize_(
    custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0* const out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes)
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
    // Union tag field: truncated uint8
    if ((offset_bits + 8U) <= capacity_bits)
    {
        out_obj->_tag_ = buffer[offset_bits / 8U] & 255U;
    }
    else
    {
        out_obj->_tag_ = 0U;
    }
    offset_bits += 8U;
    if (0U == out_obj->_tag_)  // custom_data_types_dsdl.dinosaurs.actuator.control.DifferentialDriveMotionControl.1.0 ddmc
    {
        {
            size_t _size_bytes1_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
            const int8_t _err3_ = custom_data_types_dsdl_dinosaurs_actuator_control_DifferentialDriveMotionControl_1_0_deserialize_(
                &out_obj->ddmc, &buffer[offset_bits / 8U], &_size_bytes1_);
            if (_err3_ < 0)
            {
                return _err3_;
            }
            offset_bits += _size_bytes1_ * 8U;  // Advance by the size of the nested serialized representation.
        }
    }
    else if (1U == out_obj->_tag_)  // saturated uint32 dummy
    {
        out_obj->dummy = nunavutGetU32(&buffer[0], capacity_bytes, offset_bits, 32);
        offset_bits += 32U;
    }
    else
    {
        return -NUNAVUT_ERROR_REPRESENTATION_BAD_UNION_TAG;
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    *inout_buffer_size_bytes = (size_t) (nunavutChooseMin(offset_bits, capacity_bits) / 8U);
    return NUNAVUT_SUCCESS;
}

/// Initialize an instance to default values. Does nothing if @param out_obj is NULL.
/// This function intentionally leaves inactive elements uninitialized; for example, members of a variable-length
/// array beyond its length are left uninitialized; aliased union memory that is not used by the first union field
/// is left uninitialized, etc. If full zero-initialization is desired, just use memset(&obj, 0, sizeof(obj)).
static inline void custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_initialize_(custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0* const out_obj)
{
    if (out_obj != NULL)
    {
        size_t size_bytes = 0;
        const uint8_t buf = 0;
        const int8_t err = custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_deserialize_(out_obj, &buf, &size_bytes);

        (void) err;
    }
}
/// Mark option "ddmc" active without initializing it. Does nothing if @param obj is NULL.
static inline void custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_select_ddmc_(custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0* const obj)
{
    if (obj != NULL)
    {
        obj->_tag_ = 0;
    }
}

/// Check if option "ddmc" is active. Returns false if @param obj is NULL.
static inline bool custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_is_ddmc_(const custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0* const obj)
{
    return ((obj != NULL) && (obj->_tag_ == 0));
}

/// Mark option "dummy" active without initializing it. Does nothing if @param obj is NULL.
static inline void custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_select_dummy_(custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0* const obj)
{
    if (obj != NULL)
    {
        obj->_tag_ = 1;
    }
}

/// Check if option "dummy" is active. Returns false if @param obj is NULL.
static inline bool custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0_is_dummy_(const custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Request_1_0* const obj)
{
    return ((obj != NULL) && (obj->_tag_ == 1));
}

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.control.MotionControl.Response.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.control.MotionControl.Response"
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.control.MotionControl.Response.1.0"

/// Extent is the minimum amount of memory required to hold any serialized representation of any compatible
/// version of the data type; or, on other words, it is the the maximum possible size of received objects of this type.
/// The size is specified in bytes (rather than bits) because by definition, extent is an integer number of bytes long.
/// When allocating a deserialization (RX) buffer for this data type, it should be at least extent bytes large.
/// When allocating a serialization (TX) buffer, it is safe to use the size of the largest serialized representation
/// instead of the extent because it provides a tighter bound of the object size; it is safe because the concrete type
/// is always known during serialization (unlike deserialization). If not sure, use extent everywhere.
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_EXTENT_BYTES_                    1UL
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 1UL
static_assert(custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_EXTENT_BYTES_ >= custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_,
              "Internal constraint violation");

/// saturated int8 SUCCESS = 0
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_SUCCESS (0)

/// saturated int8 NO_CONFIG_MODEL = -1
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_NO_CONFIG_MODEL (-1)

/// saturated int8 REFUSE = -2
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_REFUSE (-2)

/// saturated int8 EMERGENCY_STOP = -3
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_EMERGENCY_STOP (-3)

/// saturated int8 EMERGENCY_PAUSE = -4
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_EMERGENCY_PAUSE (-4)

/// saturated int8 LOW_POWER = -4
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_LOW_POWER (-4)

/// saturated int8 REMOTE_CONTROL = -5
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_REMOTE_CONTROL (-5)

/// saturated int8 ACCIDENT = -6
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_ACCIDENT (-6)

/// saturated int8 MODEL_NO_SUPPORT = -7
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_MODEL_NO_SUPPORT (-7)

/// saturated int8 SENSOR_FAULT = -16
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_SENSOR_FAULT (-16)

/// saturated int8 SOFTWARE_FAULT = -17
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_SOFTWARE_FAULT (-17)

/// saturated int8 HARDWARE_FAULT = -18
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_HARDWARE_FAULT (-18)

/// saturated int8 MECHANICAL_FAULT = -19
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_MECHANICAL_FAULT (-19)

/// saturated int8 OVER_LOAD = -32
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_OVER_LOAD (-32)

/// saturated int8 OVER_TEMPERATURE = -33
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_OVER_TEMPERATURE (-33)

/// saturated int8 OVER_CURRENT = -34
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_OVER_CURRENT (-34)

/// saturated int8 OVER_VOLTAGE = -35
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_OVER_VOLTAGE (-35)

/// saturated int8 OVERSPEED = -36
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_OVERSPEED (-36)

/// saturated int8 LOW_TEMPERATURE = -48
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_LOW_TEMPERATURE (-48)

/// saturated int8 LOW_VOLTAGE = -49
#define custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_LOW_VOLTAGE (-49)

typedef struct
{
    /// saturated int8 result
    int8_t result;
} custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0;

/// Serialize an instance into the provided buffer.
/// The lifetime of the resulting serialized representation is independent of the original instance.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original object where possible.
///
/// @param obj      The object to serialize.
///
/// @param buffer   The destination buffer. There are no alignment requirements.
///                 @see custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the buffer in bytes.
///                                 Upon return this value will be updated with the size of the constructed serialized
///                                 representation (in bytes); this value is then to be passed over to the transport
///                                 layer. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_serialize_(
    const custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0* const obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes)
{
    if ((obj == NULL) || (buffer == NULL) || (inout_buffer_size_bytes == NULL))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    const size_t capacity_bytes = *inout_buffer_size_bytes;
    if ((8U * (size_t) capacity_bytes) < 8UL)
    {
        return -NUNAVUT_ERROR_SERIALIZATION_BUFFER_TOO_SMALL;
    }
    // Notice that fields that are not an integer number of bytes long may overrun the space allocated for them
    // in the serialization buffer up to the next byte boundary. This is by design and is guaranteed to be safe.
    size_t offset_bits = 0U;
    {   // saturated int8 result
        // Saturation code not emitted -- native representation matches the serialized representation.
        buffer[offset_bits / 8U] = (uint8_t)(obj->result);  // C std, 6.3.1.3 Signed and unsigned integers
        offset_bits += 8U;
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad1_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err4_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad1_);  // Optimize?
        if (_err4_ < 0)
        {
            return _err4_;
        }
        offset_bits += _pad1_;
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
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_deserialize_(
    custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0* const out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes)
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
    // saturated int8 result
    out_obj->result = nunavutGetI8(&buffer[0], capacity_bytes, offset_bits, 8);
    offset_bits += 8U;
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    *inout_buffer_size_bytes = (size_t) (nunavutChooseMin(offset_bits, capacity_bits) / 8U);
    return NUNAVUT_SUCCESS;
}

/// Initialize an instance to default values. Does nothing if @param out_obj is NULL.
/// This function intentionally leaves inactive elements uninitialized; for example, members of a variable-length
/// array beyond its length are left uninitialized; aliased union memory that is not used by the first union field
/// is left uninitialized, etc. If full zero-initialization is desired, just use memset(&obj, 0, sizeof(obj)).
static inline void custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_initialize_(custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0* const out_obj)
{
    if (out_obj != NULL)
    {
        size_t size_bytes = 0;
        const uint8_t buf = 0;
        const int8_t err = custom_data_types_dsdl_dinosaurs_actuator_control_MotionControl_Response_1_0_deserialize_(out_obj, &buf, &size_bytes);

        (void) err;
    }
}

#ifdef __cplusplus
}
#endif
#endif // CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_CONTROL_MOTION_CONTROL_1_0_INCLUDED_
