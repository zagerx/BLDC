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
// Source file:   /home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/ClearErrorState.1.0.dsdl
// Generated at:  2025-03-04 13:41:26.518359 UTC
// Is deprecated: no
// Fixed port-ID: None
// Full name:     custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.ClearErrorState
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

#ifndef CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_CLEAR_ERROR_STATE_1_0_INCLUDED_
#define CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_CLEAR_ERROR_STATE_1_0_INCLUDED_

#include <nunavut/support/serialization.h>
#include <stdlib.h>

static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_TARGET_ENDIANNESS == 1693710260,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/ClearErrorState.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_OMIT_FLOAT_SERIALIZATION_SUPPORT == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/ClearErrorState.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_SERIALIZATION_ASSERTS == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/ClearErrorState.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_OVERRIDE_VARIABLE_ARRAY_CAPACITY == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/ClearErrorState.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_CAST_FORMAT == 2368206204,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/actuator/wheel_motor/ClearErrorState.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );

#ifdef __cplusplus
extern "C" {
#endif

/// This type does not have a fixed port-ID. See https://forum.opencyphal.org/t/choosing-message-and-service-ids/889
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_1_0_HAS_FIXED_PORT_ID_ false

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.ClearErrorState.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.ClearErrorState"
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.ClearErrorState.1.0"

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.ClearErrorState.Request.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.ClearErrorState.Request"
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.ClearErrorState.Request.1.0"

/// Extent is the minimum amount of memory required to hold any serialized representation of any compatible
/// version of the data type; or, on other words, it is the the maximum possible size of received objects of this type.
/// The size is specified in bytes (rather than bits) because by definition, extent is an integer number of bytes long.
/// When allocating a deserialization (RX) buffer for this data type, it should be at least extent bytes large.
/// When allocating a serialization (TX) buffer, it is safe to use the size of the largest serialized representation
/// instead of the extent because it provides a tighter bound of the object size; it is safe because the concrete type
/// is always known during serialization (unlike deserialization). If not sure, use extent everywhere.
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0_EXTENT_BYTES_                    0UL
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 0UL
static_assert(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0_EXTENT_BYTES_ >= custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_,
              "Internal constraint violation");

typedef struct
{
    uint8_t _dummy_;
} custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0;

/// Serialize an instance into the provided buffer.
/// The lifetime of the resulting serialized representation is independent of the original instance.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original object where possible.
///
/// @param obj      The object to serialize.
///
/// @param buffer   The destination buffer. There are no alignment requirements.
///                 @see custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the buffer in bytes.
///                                 Upon return this value will be updated with the size of the constructed serialized
///                                 representation (in bytes); this value is then to be passed over to the transport
///                                 layer. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0_serialize_(
    const custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0* const obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes)
{
    if ((obj == NULL) || (buffer == NULL) || (inout_buffer_size_bytes == NULL))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    *inout_buffer_size_bytes = 0U;
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
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0_deserialize_(
    custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0* const out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes)
{
    if ((out_obj == NULL) || (inout_buffer_size_bytes == NULL) || ((buffer == NULL) && (0 != *inout_buffer_size_bytes)))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    if (buffer == NULL)
    {
        buffer = (const uint8_t*)"";
    }
    *inout_buffer_size_bytes = 0U;
    return NUNAVUT_SUCCESS;
}

/// Initialize an instance to default values. Does nothing if @param out_obj is NULL.
/// This function intentionally leaves inactive elements uninitialized; for example, members of a variable-length
/// array beyond its length are left uninitialized; aliased union memory that is not used by the first union field
/// is left uninitialized, etc. If full zero-initialization is desired, just use memset(&obj, 0, sizeof(obj)).
static inline void custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0_initialize_(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0* const out_obj)
{
    if (out_obj != NULL)
    {
        size_t size_bytes = 0;
        const uint8_t buf = 0;
        const int8_t err = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Request_1_0_deserialize_(out_obj, &buf, &size_bytes);

        (void) err;
    }
}

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.ClearErrorState.Response.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.ClearErrorState.Response"
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.actuator.wheel_motor.ClearErrorState.Response.1.0"

/// Extent is the minimum amount of memory required to hold any serialized representation of any compatible
/// version of the data type; or, on other words, it is the the maximum possible size of received objects of this type.
/// The size is specified in bytes (rather than bits) because by definition, extent is an integer number of bytes long.
/// When allocating a deserialization (RX) buffer for this data type, it should be at least extent bytes large.
/// When allocating a serialization (TX) buffer, it is safe to use the size of the largest serialized representation
/// instead of the extent because it provides a tighter bound of the object size; it is safe because the concrete type
/// is always known during serialization (unlike deserialization). If not sure, use extent everywhere.
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0_EXTENT_BYTES_                    0UL
#define custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 0UL
static_assert(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0_EXTENT_BYTES_ >= custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_,
              "Internal constraint violation");

typedef struct
{
    uint8_t _dummy_;
} custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0;

/// Serialize an instance into the provided buffer.
/// The lifetime of the resulting serialized representation is independent of the original instance.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original object where possible.
///
/// @param obj      The object to serialize.
///
/// @param buffer   The destination buffer. There are no alignment requirements.
///                 @see custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the buffer in bytes.
///                                 Upon return this value will be updated with the size of the constructed serialized
///                                 representation (in bytes); this value is then to be passed over to the transport
///                                 layer. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0_serialize_(
    const custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0* const obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes)
{
    if ((obj == NULL) || (buffer == NULL) || (inout_buffer_size_bytes == NULL))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    *inout_buffer_size_bytes = 0U;
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
static inline int8_t custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0_deserialize_(
    custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0* const out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes)
{
    if ((out_obj == NULL) || (inout_buffer_size_bytes == NULL) || ((buffer == NULL) && (0 != *inout_buffer_size_bytes)))
    {
        return -NUNAVUT_ERROR_INVALID_ARGUMENT;
    }
    if (buffer == NULL)
    {
        buffer = (const uint8_t*)"";
    }
    *inout_buffer_size_bytes = 0U;
    return NUNAVUT_SUCCESS;
}

/// Initialize an instance to default values. Does nothing if @param out_obj is NULL.
/// This function intentionally leaves inactive elements uninitialized; for example, members of a variable-length
/// array beyond its length are left uninitialized; aliased union memory that is not used by the first union field
/// is left uninitialized, etc. If full zero-initialization is desired, just use memset(&obj, 0, sizeof(obj)).
static inline void custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0_initialize_(custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0* const out_obj)
{
    if (out_obj != NULL)
    {
        size_t size_bytes = 0;
        const uint8_t buf = 0;
        const int8_t err = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_ClearErrorState_Response_1_0_deserialize_(out_obj, &buf, &size_bytes);

        (void) err;
    }
}

#ifdef __cplusplus
}
#endif
#endif // CUSTOM_DATA_TYPES_DSDL_DINOSAURS_ACTUATOR_WHEEL_MOTOR_CLEAR_ERROR_STATE_1_0_INCLUDED_
