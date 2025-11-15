/**
 * @file pid.c
 * @brief PID controller implementation
 *
 * Implements:
 * - PID controller initialization
 * - Reset functionality
 * - Anti-windup control
 *
 * Copyright (c) 2023 Your Company
 * SPDX-License-Identifier: Apache-2.0
 */

 #include "./foc_pid.h"

 /**
  * @brief Initialize PID controller
  * @param pid PID control block pointer
  * @param kp Proportional gain
  * @param ki Integral gain
  * @param kc Anti-windup correction gain
  * @param outmax Maximum output limit
  * @param outmin Minimum output limit
  *
  * Initializes all PID parameters and resets internal states.
  */
 void pid_init(pid_cb_t *pid, float kp, float ki, float kc,
               float outmax, float outmin)
 {
     if (!pid) {
         return;
     }
 
     pid->kp = kp;
     pid->ki = ki;
     pid->kc = kc;
     pid->u_i = 0.0f;       /* Integral term */
     pid->satErr = 0.0f;    /* Saturation error */
     pid->out_debug = 0.0f; /* Debug output */
     pid->out_max = outmax;  /* Upper output limit */
     pid->out_min = outmin;  /* Lower output limit */
 }
 
 /**
  * @brief Reset PID controller internal states
  * @param pid PID control block pointer
  *
  * Resets integral term and saturation error while keeping gains.
  */
 void pid_reset(pid_cb_t *pid)
 {
     if (!pid) {
         return;
     }
 
     pid->out_debug = 0.0f;
     pid->u_i = 0.0f;
     pid->satErr = 0.0f;
 }
 
 /**
  * @brief Execute PID control calculation
  * @param pid PID control block pointer
  * @param tar Target value
  * @param cur Current feedback value
  * @return Controller output after limiting
  *
  * Implements:
  * - Standard PID algorithm
  * - Output saturation
  * - Anti-windup compensation
  */
 float pid_contrl(pid_cb_t *pid, float tar, float cur)
 {
     if (!pid) {
         return 0.0f;
     }
 
     float err, u_p, v_out, presat;
 
     /* Calculate error and proportional term */
     err = tar - cur;
     u_p = pid->kp * err;
 
     /* Update integral term with anti-windup */
     pid->u_i = pid->u_i + pid->ki * u_p + pid->kc * pid->satErr;
 
     /* Calculate pre-saturated output */
     presat = u_p + pid->u_i;
 
     /* Apply output saturation */
     if (presat > pid->out_max) {
         v_out = pid->out_max;
     } else if (presat < pid->out_min) {
         v_out = pid->out_min;
     } else {
         v_out = presat;
     }
 
     /* Update saturation error for anti-windup */
     pid->satErr = v_out - presat;
     pid->out_debug = v_out;
 
     return v_out;
 }