#include "stm32_pwm.h"
#include "tim.h"

void tim1_pwm_start(void) {
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}

void tim1_pwm_stop(void) {
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);

  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
}

void tim1_pwm_set(float a, float b, float c) {
  uint16_t _a, _b, _c;
  static uint32_t cout;
  if (cout++ > 800) {
    /* code */
    cout = 0;
    HAL_GPIO_TogglePin(LED02_GPIO_Port, LED02_Pin);
  }
  _a = (uint16_t)((a) * (M_TIM_ARR));
  _b = (uint16_t)((b) * (M_TIM_ARR));
  _c = (uint16_t)((c) * (M_TIM_ARR));
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, _a);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, _b);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, _c);

  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 30);
}
