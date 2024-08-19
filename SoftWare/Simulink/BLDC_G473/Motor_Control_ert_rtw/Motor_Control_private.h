/*
 * File: Motor_Control_private.h
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

#ifndef RTW_HEADER_Motor_Control_private_h_
#define RTW_HEADER_Motor_Control_private_h_
#include "rtwtypes.h"
#include "Motor_Control.h"

extern real32_T rt_modf_snf(real32_T u0, real32_T u1);
extern void open_loop_Enable(DW_open_loop *localDW);
extern void open_loop(RT_MODEL * const rtM, real32_T rtu_opl_spd, real32_T
                      rtu_opl_time, real32_T rtu_opl_tor, real32_T
                      *rty_theta_fd1, real32_T *rty_iq_ref1, DW_open_loop
                      *localDW);
extern void SCR_Enable(DW_SCR *localDW);
extern void SCR(real32_T rtu_Speed_target, real32_T rtu_Obs_Speed, real32_T
                *rty_Iq_ref, const uint32_T *rtd_close_loop_ELAPS_T, DW_SCR
                *localDW);
extern void close_loop_Enable(DW_close_loop *localDW);
extern void close_loop(RT_MODEL * const rtM, real32_T rtu_obs_spd, real32_T
  rtu_obs_theta, real32_T rtu_speed_ref, real32_T *rty_theta_fd1, real32_T
  *rty_iq_ref1, DW_close_loop *localDW);
extern void Luenberger_Init(void);
extern void Luenberger_Enable(void);
extern void Luenberger(void);
extern void _inverse_park(void);

#endif                                 /* RTW_HEADER_Motor_Control_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
