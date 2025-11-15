/**
 * @file pid.h
 * @brief PID controller interface
 *
 * Defines:
 * - PID control block structure
 * - PID controller API
 *
 * Copyright (c) 2023 Your Company
 * SPDX-License-Identifier: Apache-2.0
 */

 #ifndef __PID__H
 #define __PID__H
 
 /**
  * @struct pid_contrlblock
  * @brief PID controller control block
  */
 typedef struct pid_contrlblock {
     float kp;        /**< Proportional gain */
     float ki;        /**< Integral gain */
     float kd;        /**< Derivative gain (currently unused) */
     float kc;        /**< Anti-windup correction gain */
     float u_i;       /**< Integral term accumulator */
     float satErr;    /**< Saturation error for anti-windup */
     float out_debug; /**< Debug output value */
     float out_max;   /**< Upper output limit */
     float out_min;   /**< Lower output limit */
 } pid_cb_t;
 
 /* API Functions */
 extern float pid_contrl(pid_cb_t *pid, float tar, float cur);
 extern void pid_init(pid_cb_t *pid, float kp, float ki, float kc, 
                     float outmax, float outmin);
 extern void pid_reset(pid_cb_t *pid);
 
 #endif /* __PID__H */