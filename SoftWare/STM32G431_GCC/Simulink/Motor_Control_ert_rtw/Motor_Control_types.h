/*
 * File: Motor_Control_types.h
 *
 * Code generated for Simulink model 'Motor_Control'.
 *
 * Model version                  : 1.28
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Thu Nov 16 01:26:09 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: NXP->Cortex-M4
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Motor_Control_types_h_
#define RTW_HEADER_Motor_Control_types_h_
#include "rtwtypes.h"

/* Model Code Variants */
#ifndef DEFINED_TYPEDEF_FOR_pid_t_
#define DEFINED_TYPEDEF_FOR_pid_t_

typedef struct {
  real32_T kp;
  real32_T ki;
  real32_T kd;
  real32_T target;
} pid_t;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

#endif                                 /* RTW_HEADER_Motor_Control_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
