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
// Source file:   /home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/PortId.1.0.dsdl
// Generated at:  2025-03-04 13:41:26.494135 UTC
// Is deprecated: no
// Fixed port-ID: None
// Full name:     custom_data_types_dsdl.dinosaurs.PortId
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

#ifndef CUSTOM_DATA_TYPES_DSDL_DINOSAURS_PORT_ID_1_0_INCLUDED_
#define CUSTOM_DATA_TYPES_DSDL_DINOSAURS_PORT_ID_1_0_INCLUDED_

#include <nunavut/support/serialization.h>
#include <stdint.h>
#include <stdlib.h>

static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_TARGET_ENDIANNESS == 1693710260,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/PortId.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_OMIT_FLOAT_SERIALIZATION_SUPPORT == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/PortId.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_SERIALIZATION_ASSERTS == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/PortId.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_ENABLE_OVERRIDE_VARIABLE_ARRAY_CAPACITY == 0,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/PortId.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );
static_assert( NUNAVUT_SUPPORT_LANGUAGE_OPTION_CAST_FORMAT == 2368206204,
              "/home/zhangge/worknote/BLDC/SoftWare/Common/uavcan/custom_data_types_dsdl/dinosaurs/PortId.1.0.dsdl is trying to use a serialization library that was compiled with "
              "different language options. This is dangerous and therefore not allowed." );

#ifdef __cplusplus
extern "C" {
#endif

/// This type does not have a fixed port-ID. See https://forum.opencyphal.org/t/choosing-message-and-service-ids/889
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_HAS_FIXED_PORT_ID_ false

// +-------------------------------------------------------------------------------------------------------------------+
// | custom_data_types_dsdl.dinosaurs.PortId.1.0
// +-------------------------------------------------------------------------------------------------------------------+
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_FULL_NAME_             "custom_data_types_dsdl.dinosaurs.PortId"
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_FULL_NAME_AND_VERSION_ "custom_data_types_dsdl.dinosaurs.PortId.1.0"

/// Extent is the minimum amount of memory required to hold any serialized representation of any compatible
/// version of the data type; or, on other words, it is the the maximum possible size of received objects of this type.
/// The size is specified in bytes (rather than bits) because by definition, extent is an integer number of bytes long.
/// When allocating a deserialization (RX) buffer for this data type, it should be at least extent bytes large.
/// When allocating a serialization (TX) buffer, it is safe to use the size of the largest serialized representation
/// instead of the extent because it provides a tighter bound of the object size; it is safe because the concrete type
/// is always known during serialization (unlike deserialization). If not sure, use extent everywhere.
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_EXTENT_BYTES_                    0UL
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 0UL
static_assert(custom_data_types_dsdl_dinosaurs_PortId_1_0_EXTENT_BYTES_ >= custom_data_types_dsdl_dinosaurs_PortId_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_,
              "Internal constraint violation");

/// saturated uint16 NodeManage_1_0_ID = 1
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_NodeManage_1_0_ID (1U)

/// saturated uint16 BoardInfo_1_0_ID = 2
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_BoardInfo_1_0_ID (2U)

/// saturated uint16 ModeSetting_1_0_ID = 8
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_ModeSetting_1_0_ID (8U)

/// saturated uint16 business_cleaning_SetMode_1_0_ID = 110
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_business_cleaning_SetMode_1_0_ID (110U)

/// saturated uint16 peripheral_MemoryCommand_1_0_ID = 120
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_peripheral_MemoryCommand_1_0_ID (120U)

/// saturated uint16 peripheral_OperateRemoteDevice_1_0_ID = 121
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_peripheral_OperateRemoteDevice_1_0_ID (121U)

/// saturated uint16 peripheral_NotifyShutdown_1_0_ID = 199
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_peripheral_NotifyShutdown_1_0_ID (199U)

/// saturated uint16 peripheral_ShutdownCommand_1_0_ID = 200
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_peripheral_ShutdownCommand_1_0_ID (200U)

/// saturated uint16 peripheral_AddonsGetState_1_0_ID = 201
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_peripheral_AddonsGetState_1_0_ID (201U)

/// saturated uint16 peripheral_USBDeviceList_1_0_ID = 1020
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_peripheral_USBDeviceList_1_0_ID (1020U)

/// saturated uint16 peripheral_DeviceStatus_1_0_ID = 1021
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_peripheral_DeviceStatus_1_0_ID (1021U)

/// saturated uint16 peripheral_MovableAddons_1_0_ID = 1022
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_peripheral_MovableAddons_1_0_ID (1022U)

/// saturated uint16 Emergency_1_0_ID = 1
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_Emergency_1_0_ID (1U)

/// saturated uint16 GlobalHealth_1_0_ID = 2
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_GlobalHealth_1_0_ID (2U)

/// saturated uint16 GlobalTimeSync_1_0_ID = 3
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_GlobalTimeSync_1_0_ID (3U)

/// saturated uint16 common_KeepState_1_0_ID = 4
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_common_KeepState_1_0_ID (4U)

/// saturated uint16 ExpectStandbyState_1_0_ID = 9
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_ExpectStandbyState_1_0_ID (9U)

/// saturated uint16 peripheral_RemoteControl_1_0_ID = 2000
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_peripheral_RemoteControl_1_0_ID (2000U)

/// saturated uint16 uavcan_primitive_String_1_0_ID = 4000
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_uavcan_primitive_String_1_0_ID (4000U)

/// saturated uint16 diagnosis_TestInput_1_0_ID = 187
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_diagnosis_TestInput_1_0_ID (187U)

/// saturated uint16 diagnosis_TestOutput_1_0_ID = 188
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_diagnosis_TestOutput_1_0_ID (188U)

/// saturated uint16 diagnosis_GetStatus_1_0_ID = 189
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_diagnosis_GetStatus_1_0_ID (189U)

/// saturated uint16 bootstrap_flash_FlashPartitionOperation_1_0_PORT_ID = 255
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_flash_FlashPartitionOperation_1_0_PORT_ID (255U)

/// saturated uint16 bootstrap_flash_GetFlashPartitionTable_1_0_PORT_ID = 254
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_flash_GetFlashPartitionTable_1_0_PORT_ID (254U)

/// saturated uint16 bootstrap_flash_StartOTA_1_0_PORT_ID = 253
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_flash_StartOTA_1_0_PORT_ID (253U)

/// saturated uint16 bootstrap_flash_StopOTA_1_0_PORT_ID = 252
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_flash_StopOTA_1_0_PORT_ID (252U)

/// saturated uint16 bootstrap_flash_WriteFlash_1_0_PORT_ID = 251
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_flash_WriteFlash_1_0_PORT_ID (251U)

/// saturated uint16 bootstrap_updatee_UpdateeIterator_1_0_PORT_ID = 250
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_UpdateeIterator_1_0_PORT_ID (250U)

/// saturated uint16 bootstrap_updatee_StartOTA_1_0_PORT_ID = 249
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_StartOTA_1_0_PORT_ID (249U)

/// saturated uint16 bootstrap_updatee_WriteUpdatee_1_0_PORT_ID = 248
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_WriteUpdatee_1_0_PORT_ID (248U)

/// saturated uint16 bootstrap_updatee_UpdateeIntegarityCheck_1_0_PORT_ID = 247
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_UpdateeIntegarityCheck_1_0_PORT_ID (247U)

/// saturated uint16 bootstrap_updatee_StopOTA_1_0_PORT_ID = 246
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_StopOTA_1_0_PORT_ID (246U)

/// saturated uint16 bootstrap_updatee_PrepareOTA_1_0_PORT_ID = 245
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_PrepareOTA_1_0_PORT_ID (245U)

/// saturated uint16 bootstrap_flash_FlashPartitionOperation_1_0_ID = 255
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_flash_FlashPartitionOperation_1_0_ID (255U)

/// saturated uint16 bootstrap_flash_GetFlashPartitionTable_1_0_ID = 254
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_flash_GetFlashPartitionTable_1_0_ID (254U)

/// saturated uint16 bootstrap_flash_StartOTA_1_0_ID = 253
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_flash_StartOTA_1_0_ID (253U)

/// saturated uint16 bootstrap_flash_StopOTA_1_0_ID = 252
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_flash_StopOTA_1_0_ID (252U)

/// saturated uint16 bootstrap_flash_WriteFlash_1_0_ID = 251
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_flash_WriteFlash_1_0_ID (251U)

/// saturated uint16 bootstrap_updatee_UpdateeIterator_1_0_ID = 250
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_UpdateeIterator_1_0_ID (250U)

/// saturated uint16 bootstrap_updatee_StartOTA_1_0_ID = 249
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_StartOTA_1_0_ID (249U)

/// saturated uint16 bootstrap_updatee_WriteUpdatee_1_0_ID = 248
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_WriteUpdatee_1_0_ID (248U)

/// saturated uint16 bootstrap_updatee_UpdateeIntegarityCheck_1_0_ID = 247
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_UpdateeIntegarityCheck_1_0_ID (247U)

/// saturated uint16 bootstrap_updatee_StopOTA_1_0_ID = 246
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_StopOTA_1_0_ID (246U)

/// saturated uint16 bootstrap_updatee_PrepareOTA_1_0_ID = 245
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_bootstrap_updatee_PrepareOTA_1_0_ID (245U)

/// saturated uint16 component_InfoQuery_1_0_ID = 998
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_component_InfoQuery_1_0_ID (998U)

/// saturated uint16 component_InfoReport_1_0_ID = 999
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_component_InfoReport_1_0_ID (999U)

/// saturated uint16 sensor_lidar_Lidar_1_0_ID = 1000
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_lidar_Lidar_1_0_ID (1000U)

/// saturated uint16 sensor_lidar_Lidar_2_0_ID = 1000
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_lidar_Lidar_2_0_ID (1000U)

/// saturated uint16 sensor_presure_Presure_1_0_ID = 1001
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_presure_Presure_1_0_ID (1001U)

/// saturated uint16 sensor_sonar_Sonar_1_0_ID = 1002
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_sonar_Sonar_1_0_ID (1002U)

/// saturated uint16 sensor_imu_Imu_1_0_ID = 1003
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_imu_Imu_1_0_ID (1003U)

/// saturated uint16 sensor_binarysignal_BinarySignal_1_0_ID = 1004
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_binarysignal_BinarySignal_1_0_ID (1004U)

/// saturated uint16 sensor_binarysignal_BinarySignal_2_0_ID = 1004
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_binarysignal_BinarySignal_2_0_ID (1004U)

/// saturated uint16 sensor_powerbutton_PowerButton_1_0_ID = 1005
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_powerbutton_PowerButton_1_0_ID (1005U)

/// saturated uint16 sensor_scanner_Scanner_1_0_ID = 1006
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_scanner_Scanner_1_0_ID (1006U)

/// saturated uint16 sensor_barometer_Barometer_1_0_ID = 1007
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_barometer_Barometer_1_0_ID (1007U)

/// saturated uint16 sensor_thermometer_Thermometer_1_0_ID = 1008
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_thermometer_Thermometer_1_0_ID (1008U)

/// saturated uint16 sensor_ir_Ir_1_0_ID = 1009
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_ir_Ir_1_0_ID (1009U)

/// saturated uint16 sensor_batterystatus_BatteryStatus_1_0_ID = 1010
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_batterystatus_BatteryStatus_1_0_ID (1010U)

/// saturated uint16 sensor_powermonitor_PowerMonitor_1_0_ID = 1011
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_powermonitor_PowerMonitor_1_0_ID (1011U)

/// saturated uint16 sensor_magnetometer_Magnetometer_1_0_ID = 1012
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_magnetometer_Magnetometer_1_0_ID (1012U)

/// saturated uint16 sensor_pose_Pose_1_0_ID = 1013
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_pose_Pose_1_0_ID (1013U)

/// saturated uint16 sensor_accelerometer_Accelerometer_1_0_ID = 1014
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_accelerometer_Accelerometer_1_0_ID (1014U)

/// saturated uint16 sensor_gyroscope_Gyroscope_1_0_ID = 1015
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_gyroscope_Gyroscope_1_0_ID (1015U)

/// saturated uint16 sensor_rfid_RfidPublish_1_0_ID = 1016
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_rfid_RfidPublish_1_0_ID (1016U)

/// saturated uint16 exception_Event_1_0_ID = 1017
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_exception_Event_1_0_ID (1017U)

/// saturated uint16 exception_StatusReport_1_0_ID = 1019
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_exception_StatusReport_1_0_ID (1019U)

/// saturated uint16 WorkMode_1_0_ID = 1023
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_WorkMode_1_0_ID (1023U)

/// saturated uint16 indicator_light_Light_1_0_ID = 32
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_indicator_light_Light_1_0_ID (32U)

/// saturated uint16 indicator_beeper_Beeper_1_0_ID = 33
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_indicator_beeper_Beeper_1_0_ID (33U)

/// saturated uint16 indicator_beeper_Paramter_1_0_ID = 34
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_indicator_beeper_Paramter_1_0_ID (34U)

/// saturated uint16 actuator_wheel_motor_Odometry_1_0_ID = 1100
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_Odometry_1_0_ID (1100U)

/// saturated uint16 actuator_wheel_motor_OdometryAndVelocityPublish_1_0_ID = 1100
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_OdometryAndVelocityPublish_1_0_ID (1100U)

/// saturated uint16 actuator_wheel_motor_Status_1_0_ID = 1101
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_Status_1_0_ID (1101U)

/// saturated uint16 actuator_motion_controller_velocity_1_0_ID = 1102
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_velocity_1_0_ID (1102U)

/// saturated uint16 actuator_motion_controller_emergency_ID = 1103
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_emergency_ID (1103U)

/// saturated uint16 actuator_motion_controller_velocity_feedback_1_0_ID = 1104
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_velocity_feedback_1_0_ID (1104U)

/// saturated uint16 actuator_motion_controller_state_1_0_ID = 1105
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_state_1_0_ID (1105U)

/// saturated uint16 actuator_motion_controller_TestEnable_1_0_ID = 1106
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_TestEnable_1_0_ID (1106U)

/// saturated uint16 actuator_wheel_motor_PositionPublish_1_0_ID = 1107
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_PositionPublish_1_0_ID (1107U)

/// saturated uint16 actuator_ordinary_motor_AngularVelocityPublish_1_0_ID = 1108
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_ordinary_motor_AngularVelocityPublish_1_0_ID (1108U)

/// saturated uint16 actuator_ordinary_motor_PowerPublish_1_0_ID = 1109
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_ordinary_motor_PowerPublish_1_0_ID (1109U)

/// saturated uint16 actuator_ordinary_motor_Status_1_0_ID = 1110
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_ordinary_motor_Status_1_0_ID (1110U)

/// saturated uint16 actuator_wheel_motor_SetVelocity_1_0_ID = 100
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_SetVelocity_1_0_ID (100U)

/// saturated uint16 actuator_wheel_motor_SetPosition_1_0_ID = 101
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_SetPosition_1_0_ID (101U)

/// saturated uint16 actuator_wheel_motor_SetTorque_1_0_ID = 102
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_SetTorque_1_0_ID (102U)

/// saturated uint16 actuator_wheel_motor_RequestParameter_1_0_ID = 103
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_RequestParameter_1_0_ID (103U)

/// saturated uint16 actuator_wheel_motor_SetMode_1_0_ID = 104
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_SetMode_1_0_ID (104U)

/// saturated uint16 actuator_motion_controller_clear_error_code_1_0_ID = 105
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_clear_error_code_1_0_ID (105U)

/// saturated uint16 actuator_motion_controller_controller_parameter_1_0_ID = 106
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_controller_parameter_1_0_ID (106U)

/// saturated uint16 actuator_motion_controller_imu_parameter_1_0_ID = 107
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_imu_parameter_1_0_ID (107U)

/// saturated uint16 actuator_motion_controller_two_wheel_parameter_1_0_ID = 108
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_two_wheel_parameter_1_0_ID (108U)

/// saturated uint16 actuator_motion_controller_enable_1_0_ID = 109
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_enable_1_0_ID (109U)

/// saturated uint16 actuator_motion_controller_ControllerParameter_1_0_ID = 110
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_ControllerParameter_1_0_ID (110U)

/// saturated uint16 actuator_motion_controller_ImuAxisRotationAngleParameter_1_0_ID = 111
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_ImuAxisRotationAngleParameter_1_0_ID (111U)

/// saturated uint16 actuator_motion_controller_TwoWheelRobotMotionParameter_1_0_ID = 112
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_motion_controller_TwoWheelRobotMotionParameter_1_0_ID (112U)

/// saturated uint16 actuator_wheel_motor_Enable_1_0_ID = 113
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_Enable_1_0_ID (113U)

/// saturated uint16 actuator_wheel_motor_Parameter_1_0_ID = 114
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_Parameter_1_0_ID (114U)

/// saturated uint16 actuator_wheel_motor_ClearErrorSate_1_0_ID = 115
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_ClearErrorSate_1_0_ID (115U)

/// saturated uint16 actuator_wheel_motor_ClearErrorState_1_0_ID = 115
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_ClearErrorState_1_0_ID (115U)

/// saturated uint16 actuator_SoftwareEmergencyStop_1_0_ID = 116
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_SoftwareEmergencyStop_1_0_ID (116U)

/// saturated uint16 actuator_wheel_motor_SetTargetValue_2_0_ID = 117
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_SetTargetValue_2_0_ID (117U)

/// saturated uint16 actuator_wheel_motor_GetStatus_2_0_ID = 118
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_GetStatus_2_0_ID (118U)

/// saturated uint16 actuator_wheel_motor_SetMode_2_0_ID = 119
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_SetMode_2_0_ID (119U)

/// saturated uint16 actuator_wheel_motor_SetPiParameter_2_0_ID = 120
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_SetPiParameter_2_0_ID (120U)

/// saturated uint16 actuator_wheel_motor_SetExternalParameter_2_0_ID = 121
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_SetExternalParameter_2_0_ID (121U)

/// saturated uint16 actuator_wheel_motor_SetProtectParameter_2_0_ID = 122
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_SetProtectParameter_2_0_ID (122U)

/// saturated uint16 actuator_ordinary_motor_SetAngularVelocity_1_0_ID = 130
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_ordinary_motor_SetAngularVelocity_1_0_ID (130U)

/// saturated uint16 actuator_ordinary_motor_SetPower_1_0_ID = 131
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_ordinary_motor_SetPower_1_0_ID (131U)

/// saturated uint16 actuator_ordinary_motor_Query_1_0_ID = 132
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_ordinary_motor_Query_1_0_ID (132U)

/// saturated uint16 actuator_linear_motor_SetAbsolutePosition_1_0_ID = 140
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_linear_motor_SetAbsolutePosition_1_0_ID (140U)

/// saturated uint16 sensor_rfid_RfidSetup_1_0_ID = 133
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_rfid_RfidSetup_1_0_ID (133U)

/// saturated uint16 sensor_rfid_RfidQuery_1_0_ID = 134
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_sensor_rfid_RfidQuery_1_0_ID (134U)

/// saturated uint16 component_ParamsSetting_1_0_ID = 135
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_component_ParamsSetting_1_0_ID (135U)

/// saturated uint16 exception_Clear_1_0_ID = 136
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_exception_Clear_1_0_ID (136U)

/// saturated uint16 power_auto_charging_StopCharging_1_0_ID = 141
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_power_auto_charging_StopCharging_1_0_ID (141U)

/// saturated uint16 power_auto_charging_Status_1_0_ID = 182
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_power_auto_charging_Status_1_0_ID (182U)

/// saturated uint16 power_battery_Param_1_0_ID = 180
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_power_battery_Param_1_0_ID (180U)

/// saturated uint16 power_battery_Status_1_0_ID = 181
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_power_battery_Status_1_0_ID (181U)

/// saturated uint16 power_battery_Status_2_0_ID = 181
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_power_battery_Status_2_0_ID (181U)

/// saturated uint16 power_hibernate_state_1_0_ID = 1200
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_power_hibernate_state_1_0_ID (1200U)

/// saturated uint16 power_hibernate_command_1_0_ID = 202
#define custom_data_types_dsdl_dinosaurs_PortId_1_0_power_hibernate_command_1_0_ID (202U)

typedef struct
{
    uint8_t _dummy_;
} custom_data_types_dsdl_dinosaurs_PortId_1_0;

/// Serialize an instance into the provided buffer.
/// The lifetime of the resulting serialized representation is independent of the original instance.
/// This method may be slow for large objects (e.g., images, point clouds, radar samples), so in a later revision
/// we may define a zero-copy alternative that keeps references to the original object where possible.
///
/// @param obj      The object to serialize.
///
/// @param buffer   The destination buffer. There are no alignment requirements.
///                 @see custom_data_types_dsdl_dinosaurs_PortId_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_
///
/// @param inout_buffer_size_bytes  When calling, this is a pointer to the size of the buffer in bytes.
///                                 Upon return this value will be updated with the size of the constructed serialized
///                                 representation (in bytes); this value is then to be passed over to the transport
///                                 layer. In case of error this value is undefined.
///
/// @returns Negative on error, zero on success.
static inline int8_t custom_data_types_dsdl_dinosaurs_PortId_1_0_serialize_(
    const custom_data_types_dsdl_dinosaurs_PortId_1_0* const obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes)
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
static inline int8_t custom_data_types_dsdl_dinosaurs_PortId_1_0_deserialize_(
    custom_data_types_dsdl_dinosaurs_PortId_1_0* const out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes)
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
static inline void custom_data_types_dsdl_dinosaurs_PortId_1_0_initialize_(custom_data_types_dsdl_dinosaurs_PortId_1_0* const out_obj)
{
    if (out_obj != NULL)
    {
        size_t size_bytes = 0;
        const uint8_t buf = 0;
        const int8_t err = custom_data_types_dsdl_dinosaurs_PortId_1_0_deserialize_(out_obj, &buf, &size_bytes);

        (void) err;
    }
}

#ifdef __cplusplus
}
#endif
#endif // CUSTOM_DATA_TYPES_DSDL_DINOSAURS_PORT_ID_1_0_INCLUDED_
