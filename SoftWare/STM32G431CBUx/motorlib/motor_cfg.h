#ifndef MOTOR_CFG_H
#define MOTOR_CFG_H
#define PWM_FREQ 10000.0f // 10KHZ

#define PWM_CYCLE        0.0001f // 100us
#define SPEED_LOOP_CYCLE 0.001f  // 1ms
#define POS_LOOP_CYCLE   0.01f   // 10ms

#define POSITION_LOOP_DIVIDER 100
#define VELOCITY_LOOP_DIVIDER 10

/*  电流采样  */
#define BOARD_ADC_RES           4096     // 分辨率 12-bit
#define BOARD_ADC_VREF_V        3.3f     // 参考电压，单位 V
#define BOARD_SHUNT_R_OHM       0.005f   // 分流电阻，单位 Ω
#define BOARD_OPAMP_GAIN        7.33f    // 运放闭环增益
#define BOARD_ADC_V2I_A_PER_LSB 0.02196f // A/LSB

/* 编码器相关 */
#define ENC_CPR 16383U /* 单圈绝对值编码器码值（14-bit） */

/* 开环试运行 */
#define OPENLOOP_VOLTAGE_V 0.3f  /* 试运行定子电压，单位 V  */
#define OPENLOOP_VBUS_V    24.0f /* 自开环强拖时母线电压 单位 V  */

#endif
