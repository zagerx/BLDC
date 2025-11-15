/*
电机极对数辨识模块
*/
#include "motor_pp_ident.h"
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "math.h"
#include "motor.h"

#include "coord_transform.h"
#include "svpwm.h"

#undef M_PI
#define M_PI 3.1415926F

void pp_ident_start(struct device *motor) {
  struct motor_config *m_conf = (struct motor_config *)motor->config;
  struct feedback_config *fb_config = m_conf->feedback->config;
  struct pp_ident *pp = m_conf->pole_pairs_ident;
  pp->data.done = false;
  pp->internal_data.running = true;
  pp->internal_data.time_acc = 0.0f;
  pp->internal_data.elec_angle = 0.0f;

  // 记录开始时的编码器原始值
  pp->data.raw_start = fb_config->get_raw();
}

void pp_ident_update(struct device *motor, float dt) {
  struct motor_config *m_conf = (struct motor_config *)motor->config;
  struct feedback_config *fb_config = m_conf->feedback->config;
  struct device *interver = m_conf->inverter;
  struct pp_ident *pp = m_conf->pole_pairs_ident;

  if (!pp->internal_data.running)
    return;

  pp->internal_data.time_acc += dt;

  /*---------------------------------------------------------
   * Step1：用开环方式更新电角度（内部自转）
   *--------------------------------------------------------*/
  pp->internal_data.elec_angle += pp->config.openloop_speed * dt;
  // 限制在 0~2π 内，但实际无所谓
  if (pp->internal_data.elec_angle > M_PI * 2)
    pp->internal_data.elec_angle -= M_PI * 2;

  /*---------------------------------------------------------
   * Step2：把此电角度作为电压指令发送到逆变器（假代码）
   *--------------------------------------------------------*/
  float iq_cmd = 0.5f; // 给一个固定电流让电机能稳定拉转
  float id_cmd = 0.0f;
  float ualpha, ubeta;
  float sin_val, cos_val;
  float d_abc[3];
  sin_cos_f32(pp->internal_data.elec_angle, &sin_val, &cos_val);
  inv_park_f32(id_cmd, iq_cmd, &ualpha, &ubeta, sin_val, cos_val);
  svm_set(ualpha, ubeta, d_abc);
  inverter_set_3phase_voltages(interver, d_abc[0], d_abc[1], d_abc[1]);

  /*---------------------------------------------------------
   * Step3：判断是否到达总时间
   *--------------------------------------------------------*/
  if (pp->internal_data.time_acc < pp->config.duration)
    return;

  /*---------------------------------------------------------
   * Step4：识别结束，读取最终编码器值
   *--------------------------------------------------------*/
  pp->data.raw_end = fb_config->get_raw();

  /*
   * 注意 raw 是机械角度，可能回绕，所以必须展开
   */
  int32_t delta = pp->data.raw_end - pp->data.raw_start;

  // 展开到连续空间，使变化量接近真实
  if (delta > pp->config.encoder_max / 2)
    delta -= pp->config.encoder_max;
  else if (delta < -pp->config.encoder_max / 2)
    delta += pp->config.encoder_max;

  pp->data.raw_delta = delta;

  /*---------------------------------------------------------
   * 计算机械旋转圈数
   *--------------------------------------------------------*/
  pp->data.mech_rounds = (float)delta / pp->config.encoder_max;
  /*---------------------------------------------------------
   * 我们人为施加的电角度旋转圈数 = 施加电角速度 * 时间 / (2π)
   *--------------------------------------------------------*/
  pp->data.electrical_rounds =
      pp->config.openloop_speed * pp->config.duration / (2.0f * M_PI);

  /*
   * ★ pp = 电角度圈数 / 机械角度圈数
   */
  pp->data.pole_pairs =
      (uint16_t)roundf(pp->data.electrical_rounds / pp->data.mech_rounds);

  pp->data.done = true;
  pp->internal_data.running = false;
}
