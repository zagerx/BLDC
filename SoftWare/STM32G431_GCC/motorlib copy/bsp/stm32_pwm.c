#include "stm32_pwm.h"
#include "tim.h"

#define TIMER1_CLK_MHz 168                                      // 定时器时钟频率
#define PWM_FREQUENCY 20000                                     // PWM频率20KHz
#define PWM_MEASURE_PERIOD (float)(1.0f / (float)PWM_FREQUENCY) // PWM周期
#define CURRENT_MEASURE_HZ PWM_FREQUENCY                        // 电流频率
#define CURRENT_MEASURE_PERIOD \
  (float)(1.0f / (float)CURRENT_MEASURE_HZ) // 电流周期
#define PWM_PERIOD_CYCLES                           \
  (uint16_t)((TIMER1_CLK_MHz * (uint32_t)1000000u / \
              ((uint32_t)(PWM_FREQUENCY))) &        \
             0xFFFE)                                     // 定时器1的周期数
#define PWM_ARR (uint16_t)(PWM_PERIOD_CYCLES / 2u)       // 3500
#define VEL_POS_HZ 5000                                  // 速度位置环频率
#define VEL_POS_PERIOD (float)(1.0f / (float)VEL_POS_HZ) // 速度位置环周期
#define CURRENT_CALIBRATION_DURATION 1000u               // 电流校准时间
#define DEADTIME_COMP 20u                                // 死区时间补偿
#define TS 1.0f                                          // 采样周期 (将占空比限制在0-1之间)

#define set_dtc_a(value) \
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, value) // 设置A相PWM占空比
#define set_dtc_b(value) \
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, value) // 设置B相PWM占空比
#define set_dtc_c(value) \
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, value) // 设置C相PWM占空比

void tim1_pwm_start(void)
{
  set_dtc_a(PWM_ARR >> 1); // 设置通道1的占空比
  set_dtc_b(PWM_ARR >> 1); // 设置通道2的占空比
  set_dtc_c(PWM_ARR >> 1); // 设置通道3的占空比

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
}

void tim1_pwm_stop(void)
{
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);

  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
}

void tim1_pwm_set(float a, float b, float c)
{
  set_dtc_a((uint16_t)(a * PWM_ARR)); // 设置通道1的占空比
  set_dtc_b((uint16_t)(b * PWM_ARR)); // 设置通道2的占空比
  set_dtc_c((uint16_t)(c * PWM_ARR)); // 设置通道3的占空比
}