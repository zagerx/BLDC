#ifndef MOTOR_CFG_H
#define MOTOR_CFG_H
#define PWM_FREQ         10000.0f // 10KHZ
#define PWM_CYCLE        0.0001f  // 100us
#define SPEED_LOOP_CYCLE 0.002f   // 2ms

/*  电流采样  */
#define BOARD_ADC_RES           4096     // 分辨率 12-bit
#define BOARD_ADC_VREF_V        3.3f     // 参考电压，单位 V
#define BOARD_SHUNT_R_OHM       0.005f   // 分流电阻，单位 Ω
#define BOARD_OPAMP_GAIN        7.33f    // 运放闭环增益
#define BOARD_ADC_V2I_A_PER_LSB 0.02196f // A/LSB

/* 编码器相关 */
#define ENC_CPR 16383U /* 单圈绝对值编码器码值（14-bit） */

/* 开环试运行 */
#define OPENLOOP_SPEED_RPM 5.0f  /* 试运行目标转速，单位 RPM */
#define OPENLOOP_VOLTAGE_V 0.03f /* 试运行定子电压，单位 V  */
#define ROTATE_DURATION_S  6.0f  /* 试运行旋转时长，单位 s   */
#define ALIGN_DURATION_S   1.0f  /* 初始对齐时长，单位 s     */
#endif
