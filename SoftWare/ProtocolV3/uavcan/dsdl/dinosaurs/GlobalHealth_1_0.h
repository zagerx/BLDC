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
// Source file:   /home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/GlobalHealth.1.0.dsdl
// Generated at:  2025-03-04 13:41:26.473769 UTC
// Is deprecated: no
// Fixed port-ID: None
// Full name:     custom_data_types_dsdl.dinosaurs.GlobalHealth
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

#ifndef CUSTOM_DATA_TYPES_DSDL_DINOSAURS_GLOBAL_HEALTH_1_0_INCLUDED_
#define CUSTOM_DATA_TYPES_DSDL_DINOSAURS_GLOBAL_HEALTH_1_0_INCLUDED_

#include <nunavut/support/serialization.h>
#include <stdint.h>
#include <stdlib.h>
#include <uavcan/node/Health_1_0.h>
#include <uavcan/primitive/String_1_0.h>

static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_TARGET_ENDIANNESS == 1693710260,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/GlobalHealth.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_OMIT_FLOAT_SERIALIZATION_SUPPORT == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/GlobalHealth.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_SERIALIZATION_ASSERTS == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/GlobalHealth.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_OVERRIDE_VARIABLE_ARRAY_CAPACITY == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/GlobalHealth.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_CAST_FORMAT == 2368206204,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/GlobalHealth.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );

#ifdef __cplusplus
extern "C" {
#endif

/// This type does not have a fixed port-ID. See https://forum.opencyphal.org/t/choosing-message-and-service-ids/889
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_HAS_FIXED_PORT_ID_ false

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.GlobalHealth.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.GlobalHealth"
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.GlobalHealth.1.0"

/// Extent is the minimum amount of memory required to hold any serialized representation of any compatible
/// version of the data type; or, on other words, it is the the maximum possible size of received objects of this type.
/// The size is specified in bytes (rather than bits) because by definition, extent is an integer number of bytes long.
/// When allocating a deserialization (RX) buffer for this data type, it should be at least extent bytes large.
/// When allocating a serialization (TX) buffer, it is safe to use the size of the largest serialized representation
/// instead of the extent because it provides a tighter bound of the object size; it is safe because the concrete type
/// is always known during serialization (unlike deserialization). If not sure, use extent everywhere.
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_EXTENT_BYTES_                    1024UL
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 552UL
static_assert(custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_EXTENT_BYTES_ >= custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_,
              "Internal constraint violation");

/// saturated uint16 ACCELEROMETER = 1
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_ACCELEROMETER (1U)

/// saturated uint16 GYROSCOPE = 2
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_GYROSCOPE (2U)

/// saturated uint16 IMU = 3
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_IMU (3U)

/// saturated uint16 IR = 4
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_IR (4U)

/// saturated uint16 LIDAR = 5
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_LIDAR (5U)

/// saturated uint16 BAROMETER = 6
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_BAROMETER (6U)

/// saturated uint16 SONAR = 7
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_SONAR (7U)

/// saturated uint16 THERMOMETER = 8
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_THERMOMETER (8U)

/// saturated uint16 ENCODER = 9
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_ENCODER (9U)

/// saturated uint16 LINEAR_MOTOR = 21
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_LINEAR_MOTOR (21U)

/// saturated uint16 ORDINARY_MOTOR = 22
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_ORDINARY_MOTOR (22U)

/// saturated uint16 WHEELMOTOR = 23
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_WHEELMOTOR (23U)

/// saturated uint16 BEEPER = 31
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_BEEPER (31U)

/// saturated uint16 LIGHT = 32
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_LIGHT (32U)

/// saturated uint16 BATTERY = 41
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_BATTERY (41U)

/// saturated uint16 CHARGER = 42
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_CHARGER (42U)

/// Array metadata for: saturated uint8[<=32] error_code
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_error_code_ARRAY_CAPACITY_           32U
#define custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_error_code_ARRAY_IS_VARIABLE_LENGTH_ true

typedef struct
{
    /// uavcan.primitive.String.1.0 error_source
    uavcan_primitive_String_1_0 error_source;

    /// saturated uint16 device_type
    uint16_t device_type;

    /// uavcan.node.Health.1.0 health
    uavcan_node_Health_1_0 health;

    /// saturated uint8[<=32] error_code
    struct  /// Array address equivalence guarantee: &elements[0] == &error_code
    {
        uint8_t elements[custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_error_code_ARRAY_CAPACITY_];
        size_t count;
    } error_code;

    /// uavcan.primitive.String.1.0 error_message
    uavcan_primitive_String_1_0 error_message;
} custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0;

/// Serialize an instance into the provided buffer.
/// The lifetime of the resulting serialized representation is independent of the original instance.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original object where possible.
///
/// @param obj      The object to serialize.
///
/// @param buffer   The destination buffer. There are no alignment requirements.
///                 @see custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the buffer in bytes.
///                                 Upon return this value will be updated with the size of the constructed serialized
///                                 representation (in bytes); this value is then to be passed over to the transport
///                                 layer. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_serialize_(
    const custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0* const obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes)
{
    if ((obj == NULL) || (buffer == NULL) || (inout_buffer_size_bytes == NULL))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    const size_t capacity_bytes = *inout_buffer_size_bytes;
    if ((8U * (size_t) capacity_bytes) < 4416UL)
    {
        return -NUNAVUT_ERROR_SERIALIZATION_BUFFER_TOO_SMALL;
    }
    // Notice that fields that are not an integer number of bytes long may overrun the space allocated for them
    // in the serialization buffer up to the next byte boundary. This is by design and is guaranteed to be safe.
    size_t offset_bits = 0U;
    {   // uavcan.primitive.String.1.0 error_source
        size_t _size_bytes0_ = 258UL;  // Nested object (max) size, in bytes.
        int8_t _err0_ = uavcan_primitive_String_1_0_serialize_(
            &obj->error_source, &buffer[offset_bits / 8U], &_size_bytes0_);
        if (_err0_ < 0)
        {
            return _err0_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes0_ * 8U;  // Advance by the size of the nested object.
    }
    {   // saturated uint16 device_type
        // Saturation code not emitted -- native representation matches the serialized representation.
        const int8_t _err1_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, obj->device_type, 16U);
        if (_err1_ < 0)
        {
            return _err1_;
        }
        offset_bits += 16U;
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
    {   // uavcan.node.Health.1.0 health
        size_t _size_bytes1_ = 1UL;  // Nested object (max) size, in bytes.
        int8_t _err3_ = uavcan_node_Health_1_0_serialize_(
            &obj->health, &buffer[offset_bits / 8U], &_size_bytes1_);
        if (_err3_ < 0)
        {
            return _err3_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes1_ * 8U;  // Advance by the size of the nested object.
    }
    {   // saturated uint8[<=32] error_code
        if (obj->error_code.count > 32)
        {
            return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
        }
        // Array length prefix: truncated uint8
        buffer[offset_bits / 8U] = (uint8_t)(obj->error_code.count);  // C std, 6.3.1.3 Signed and unsigned integers
        offset_bits += 8U;
        for (size_t _index0_ = 0U; _index0_ < obj->error_code.count; ++_index0_)
        {
            // Saturation code not emitted -- native representation matches the serialized representation.
            buffer[offset_bits / 8U] = (uint8_t)(obj->error_code.elements[_index0_]);  // C std, 6.3.1.3 Signed and unsigned integers
            offset_bits += 8U;
        }
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
    {   // uavcan.primitive.String.1.0 error_message
        size_t _size_bytes2_ = 258UL;  // Nested object (max) size, in bytes.
        int8_t _err5_ = uavcan_primitive_String_1_0_serialize_(
            &obj->error_message, &buffer[offset_bits / 8U], &_size_bytes2_);
        if (_err5_ < 0)
        {
            return _err5_;
        }
        // It is assumed that we know the exact type of the serialized entity, hence we expect the size to match.
        offset_bits += _size_bytes2_ * 8U;  // Advance by the size of the nested object.
    }
    if (offset_bits % 8U != 0U)  // Pad to 8 bits. TODO: Eliminate redundant padding checks.
    {
        const uint8_t _pad2_ = (uint8_t)(8U - offset_bits % 8U);
        const int8_t _err6_ = nunavutSetUxx(&buffer[0], capacity_bytes, offset_bits, 0U, _pad2_);  // Optimize?
        if (_err6_ < 0)
        {
            return _err6_;
        }
        offset_bits += _pad2_;
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
static inline int8_t custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_deserialize_(
    custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0* const out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes)
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
    // uavcan.primitive.String.1.0 error_source
    {
        size_t _size_bytes3_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err7_ = uavcan_primitive_String_1_0_deserialize_(
            &out_obj->error_source, &buffer[offset_bits / 8U], &_size_bytes3_);
        if (_err7_ < 0)
        {
            return _err7_;
        }
        offset_bits += _size_bytes3_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    // saturated uint16 device_type
    out_obj->device_type = nunavutGetU16(&buffer[0], capacity_bytes, offset_bits, 16);
    offset_bits += 16U;
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.node.Health.1.0 health
    {
        size_t _size_bytes4_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err8_ = uavcan_node_Health_1_0_deserialize_(
            &out_obj->health, &buffer[offset_bits / 8U], &_size_bytes4_);
        if (_err8_ < 0)
        {
            return _err8_;
        }
        offset_bits += _size_bytes4_ * 8U;  // Advance by the size of the nested serialized representation.
    }
    // saturated uint8[<=32] error_code
    // Array length prefix: truncated uint8
    if ((offset_bits + 8U) <= capacity_bits)
    {
        out_obj->error_code.count = buffer[offset_bits / 8U] & 255U;
    }
    else
    {
        out_obj->error_code.count = 0U;
    }
    offset_bits += 8U;
    if (out_obj->error_code.count > 32U)
    {
        return -NUNAVUT_ERROR_REPRESENTATION_BAD_ARRAY_LENGTH;
    }
    for (size_t _index1_ = 0U; _index1_ < out_obj->error_code.count; ++_index1_)
    {
        if ((offset_bits + 8U) <= capacity_bits)
        {
            out_obj->error_code.elements[_index1_] = buffer[offset_bits / 8U] & 255U;
        }
        else
        {
            out_obj->error_code.elements[_index1_] = 0U;
        }
        offset_bits += 8U;
    }
    offset_bits = (offset_bits + 7U) & ~(size_t) 7U;  // Align on 8 bits.
    // uavcan.primitive.String.1.0 error_message
    {
        size_t _size_bytes5_ = (size_t)(capacity_bytes - nunavutChooseMin((offset_bits / 8U), capacity_bytes));
        const int8_t _err9_ = uavcan_primitive_String_1_0_deserialize_(
            &out_obj->error_message, &buffer[offset_bits / 8U], &_size_bytes5_);
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
static inline void custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_initialize_(custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0* const out_obj)
{
    if (out_obj != NULL)
    {
        size_t size_bytes = 0;
        const uint8_t buf = 0;
        const int8_t err = custom_data_types_dsdl_dinosaurs_GlobalHealth_1_0_deserialize_(out_obj, &buf, &size_bytes);

        (void) err;
    }
}

#ifdef __cplusplus
}
#endif
#endif // CUSTOM_DATA_TYPES_DSDL_DINOSAURS_GLOBAL_HEALTH_1_0_INCLUDED_

