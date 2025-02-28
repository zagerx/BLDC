/*
 * Syrius Robotics Ltd. Co. CONFIDENTIAL
 *
 * Unpublished Copyright (c) 2018 - 2023 [Syrius Robotics Ltd. Co.],
 * All Rights Reserved.
 *
 * Author: lilinjun lilinjun@syriusrobotics.com
 * Author: Cherish guoweiwei@syriusrobotics.com
 */

#ifndef __DINOSAURS_DEFAULT_PORT_ID_H__
#define __DINOSAURS_DEFAULT_PORT_ID_H__

/* global service id */
#define dinosaurs_NodeManage_1_0_FIXED_PORT_ID_ 1U
#define dinosaurs_NodeManage_1_1_FIXED_PORT_ID_ 1U
#define dinosaurs_BoardInfo_1_0_FIXED_PORT_ID_  2U
#define dinosaurs_ModeSetting_1_0_FIXED_PORT_ID_     8U
#define dinosaurs_business_cleaning_SetMode_1_0_FIXED_PORT_ID_  110U
#define dinosaurs_peripheral_MemoryCommand_1_0_FIXED_PORT_ID_       120U
#define dinosaurs_peripheral_OperateRemoteDevice_1_0_FIXED_PORT_ID_ 121U
#define dinosaurs_peripheral_NotifyShutdown_1_0_FIXED_PORT_ID_  199
#define dinosaurs_peripheral_ShutdownCommand_1_0_FIXED_PORT_ID_ 200
#define dinosaurs_peripheral_AddonsGetState_1_0_FIXED_PORT_ID_ 201

/* global message id */
#define dinosaurs_Emergency_1_0_FIXED_PORT_ID_    1U
#define dinosaurs_Emergency_2_0_FIXED_PORT_ID_    1U
#define dinosaurs_GlobalHealth_1_0_FIXED_PORT_ID_ 2U
#define dinosaurs_TimeSync_1_0_FIXED_PORT_ID_     3U
#define dinosaurs_common_KeepState_1_0_FIXED_PORT_ID_   4U
#define dinosaurs_ExpectStandbyState_1_0_FIXED_PORT_ID_ 9U
#define dinosaurs_peripheral_USBDeviceList_1_0_FIXED_PORT_ID_ 1020U
#define dinosaurs_peripheral_DeviceStatus_1_0_FIXED_PORT_ID_  1021U
#define dinosaurs_peripheral_MovableAddons_1_0_FIXED_PORT_ID_ 1022U
#define dinosaurs_peripheral_RemoteControl_1_0_FIXED_PORT_ID_ 2000U
#define uavcan_primitive_String_1_0_FIXED_PORT_ID_            4000U

/* updatee object OTA mode */
#define dinosaurs_bootstrap_updatee_UpdateeIterator_1_0_FIXED_PORT_ID_        250U
#define dinosaurs_bootstrap_updatee_StartOTA_1_0_FIXED_PORT_ID_               249U
#define dinosaurs_bootstrap_updatee_WriteUpdatee_1_0_FIXED_PORT_ID_           248U
#define dinosaurs_bootstrap_updatee_UpdateeIntegarityCheck_1_0_FIXED_PORT_ID_ 247U
#define dinosaurs_bootstrap_updatee_StopOTA_1_0_FIXED_PORT_ID_                246U
#define dinosaurs_bootstrap_updatee_PrepareOTA_1_0_FIXED_PORT_ID_             245U


#define dinosaurs_component_InfoQuery_1_0_FIXED_PORT_ID_ 998U
#define dinosaurs_component_InfoReport_1_0_FIXED_PORT_ID_ 999U

/* sensors-message-publish-id */
#define dinosaurs_sensor_lidar_Lidar_1_0_FIXED_PORT_ID_                 1000U
#define dinosaurs_sensor_lidar_Lidar_2_0_FIXED_PORT_ID_                 1000U
#define dinosaurs_sensor_pressure_Pressure_1_0_FIXED_PORT_ID_           1001U
#define dinosaurs_sensor_sonar_Sonar_1_0_FIXED_PORT_ID_                 1002U
#define dinosaurs_sensor_imu_Imu_1_0_FIXED_PORT_ID_                     1003U
#define dinosaurs_sensor_binarysignal_BinarySignal_1_0_FIXED_PORT_ID_   1004U
#define dinosaurs_sensor_binarysignal_BinarySignal_2_0_FIXED_PORT_ID_   1004U
#define dinosaurs_sensor_powerbutton_PowerButton_1_0_FIXED_PORT_ID_     1005U
#define dinosaurs_sensor_scanner_Scanner_1_0_FIXED_PORT_ID_             1006U
#define dinosaurs_sensor_barometer_Barometer_1_0_FIXED_PORT_ID_         1007U
#define dinosaurs_sensor_thermometer_Thermometer_1_0_FIXED_PORT_ID_     1008U
#define dinosaurs_sensor_ir_Ir_1_0_FIXED_PORT_ID_                       1009U
#define dinosaurs_sensor_powermonitor_PowerMonitor_1_0_FIXED_PORT_ID_   1011U
#define dinosaurs_sensor_magnetometer_Magnetometer_1_0_FIXED_PORT_ID_   1012U
#define dinosaurs_sensor_button_Button_1_0_FIXED_PORT_ID_               1013U
#define dinosaurs_sensor_accelerometer_Accelerometer_1_0_FIXED_PORT_ID_ 1014U
#define dinosaurs_sensor_gyroscope_Gyroscope_1_0_FIXED_PORT_ID_         1015U
#define dinosaurs_sensor_rfid_RfidPublish_1_0_FIXED_PORT_ID_            1016U

/* exception-message-id */
#define dinosaurs_exception_Event_1_0_FIXED_PORT_ID_  1017
#define dinosaurs_exception_StatusReport_1_0_FIXED_PORT_ID_  1019

#define dinosaurs_WorkMode_1_0_FIXED_PORT_ID_ 1023

/* indicator-service-id */
#define dinosaurs_indicator_light_Light_1_0_FIXED_PORT_ID_   32U
#define dinosaurs_indicator_beeper_Beeper_1_0_FIXED_PORT_ID_ 33U
#define dinosaurs_indicator_beeper_Paramter_1_0_FIXED_PORT_ID_ 34U

/* actuator-message-publish-id */
#define dinosaurs_actuator_wheel_motor_OdometryAndVelocityPublish_1_0_FIXED_PORT_ID_          1100U
#define dinosaurs_actuator_wheel_motor_Status_1_0_FIXED_PORT_ID_                              1101U
#define dinosaurs_actuator_motion_controller_RobotMotionVelocity_1_0_FIXED_PORT_ID_           1102U
#define dinosaurs_actuator_motion_controller_RobotMotionSoftEmergencyStop_1_0_FIXED_PORT_ID_  1103U
#define dinosaurs_actuator_motion_controller_CurrentVelocityFeedback_1_0_FIXED_PORT_ID_       1104U
#define dinosaurs_actuator_motion_controller_State_1_0_FIXED_PORT_ID_                         1105U
#define dinosaurs_actuator_motion_controller_TestEnable_1_0_FIXED_PORT_ID_                    1106U
#define dinosaurs_actuator_wheel_motor_PositionPublish_1_0_FIXED_PORT_ID_                     1107U
#define dinosaurs_actuator_ordinary_motor_AngularVelocityPublish_1_0_FIXED_PORT_ID_           1108U
#define dinosaurs_actuator_ordinary_motor_PowerPublish_1_0_FIXED_PORT_ID_                     1109U
#define dinosaurs_actuator_ordinary_motor_Status_1_0_FIXED_PORT_ID_                           1110U

// actuator-service-id
#define dinosaurs_actuator_wheel_motor_SetVelocity_1_0_FIXED_PORT_ID_                100U
#define dinosaurs_actuator_wheel_motor_SetPosition_1_0_FIXED_PORT_ID_                101U
#define dinosaurs_actuator_wheel_motor_SetTorque_1_0_FIXED_PORT_ID_                  102U
#define dinosaurs_actuator_wheel_motor_SetMode_1_0_FIXED_PORT_ID_                    104U
#define dinosaurs_actuator_motion_controller_ControllerParameter_1_0_FIXED_PORT_ID_  110U
#define dinosaurs_actuator_motion_controller_ImuAxisRotationAngleParameter_1_0_FIXED_PORT_ID_ 111U
#define dinosaurs_actuator_motion_controller_TwoWheelRobotMotionParameter_1_0_FIXED_PORT_ID_  112U
#define dinosaurs_actuator_motion_controller_Enable_1_0_FIXED_PORT_ID_               113U
#define dinosaurs_actuator_wheel_motor_Enable_1_0_FIXED_PORT_ID_                     113U
#define dinosaurs_actuator_wheel_motor_Parameter_1_0_FIXED_PORT_ID_                  114U
#define dinosaurs_actuator_motion_controller_ClearErrorState_1_0_FIXED_PORT_ID_      115U
#define dinosaurs_actuator_wheel_motor_ClearErrorState_1_0_FIXED_PORT_ID_            115U
#define dinosaurs_actuator_SoftwareEmergencyStop_1_0_FIXED_PORT_ID_                  116U
#define dinosaurs_actuator_wheel_motor_SetTargetValue_2_0_FIXED_PORT_ID_             117U
#define dinosaurs_actuator_wheel_motor_GetStatus_2_0_FIXED_PORT_ID_                  118U
#define dinosaurs_actuator_wheel_motor_SetMode_2_0_FIXED_PORT_ID_                    119U
#define dinosaurs_actuator_wheel_motor_SetPiParameter_2_0_FIXED_PORT_ID_             120U
#define dinosaurs_actuator_wheel_motor_SetExternalParameter_2_0_FIXED_PORT_ID_       121U
#define dinosaurs_actuator_wheel_motor_SetProtectParameter_2_0_FIXED_PORT_ID_        122U
#define dinosaurs_actuator_control_MotionControl_1_0_FIXED_PORT_ID_                  123U
#define dinosaurs_actuator_parameter_MotionModelParameters_1_0_FIXED_PORT_ID_        124U


#define dinosaurs_actuator_ordinary_motor_SetAngularVelocity_1_0_FIXED_PORT_ID_      130U
#define dinosaurs_actuator_ordinary_motor_SetPower_1_0_FIXED_PORT_ID_                131U
#define dinosaurs_actuator_ordinary_motor_Query_1_0_FIXED_PORT_ID_                   132U

#define dinosaurs_sensor_rfid_RfidSetup_1_0_FIXED_PORT_ID_                           133U
#define dinosaurs_sensor_rfid_RfidQuery_1_0_FIXED_PORT_ID_                           134U

// component
#define dinosaurs_component_ParamsSetting_1_0_FIXED_PORT_ID_                         135U

/* exception-message-id */
#define dinosaurs_exception_Clear_1_0_FIXED_PORT_ID_                                 136U

#define dinosaurs_actuator_linear_motor_SetAbsolutePosition_1_0_FIXED_PORT_ID_       140U

/* power-service-id */
#define dinosaurs_power_battery_Parameters_1_0_FIXED_PORT_ID_  180U
#define dinosaurs_power_battery_Status_1_0_FIXED_PORT_ID_      181U
#define dinosaurs_power_battery_Status_2_0_FIXED_PORT_ID_      181U
#define dinosaurs_power_auto_charging_Status_1_0_FIXED_PORT_ID_        182U
#define dinosaurs_power_auto_charging_StopCharging_1_0_FIXED_PORT_ID_  141U
#define dinosaurs_power_hibernate_state_1_0_FIXED_PORT_ID_     1200U
#define dinosaurs_power_hibernate_command_1_0_FIXED_PORT_ID_   202U

/* diagnosis */
#define dinosaurs_diagnosis_TestInput_1_0_FIXED_PORT_ID_       187U
#define dinosaurs_diagnosis_TestOutput_1_0_FIXED_PORT_ID_      188U
#define dinosaurs_diagnosis_GetStatus_1_0_FIXED_PORT_ID_       189U

#endif
