#include "motor.h"
#include "device.h"
#include "currsmp.h"
#include "feedback.h"
#include "inverter.h"
#include "svpwm.h"
#include "coord_transform.h"

#include "statemachine.h"
#include "motor_mode.h"

extern struct device motor1;

void foc_curr_regulator(void)
{
  // struct device *motor = (struct device *)ctx;
  struct device *motor = &motor1;
  struct motor_config *m_cfg = motor->config;
  struct device *feedback = m_cfg->feedback;
  struct device *currsmp = m_cfg->currsmp;
  struct device *inverer = m_cfg->inverter;

  // feedback_update_angle_vec(feedback);
  // currsmp_updata(currsmp);

  static float self_theta = 360.0f;
  self_theta -= 0.001f;
  if (self_theta < 0.0f)
  {
    self_theta = 360.0f;
  }
  
  float sin_val, cos_val;
  sin_cos_f32(self_theta, &sin_val, &cos_val);

  float ud, uq;
  float ualpha, ubeta;
  ud = 0.0f;
  uq = 0.02f;
  inv_park_f32(ud, uq, &ualpha, &ubeta, sin_val, cos_val);
  float duty[3];
  svm_set(ualpha, ubeta, duty);

  inverter_set_3phase_voltages(inverer, duty[0], duty[1], duty[2]);
}
void motor_init(struct device *motor)
{
  struct motor_data *m_data = motor->data;
  fsm_cb_t *fsm = m_data->fsm_mode;
  if (!fsm->p1)
  {
    fsm->p1 = (void *)motor;
  }

}
void motor_task(struct device *motor)
{
  struct motor_data *m_data = motor->data;
  if(m_data->flag == PARAM_NEVER_LOADED)
  {
    TRAN_STATE(m_data->fsm_mode,motor_calibration_mode);
  }else{

  }

  for(;;)
  {
    DISPATCH_FSM(m_data->fsm_mode);
  }
}


