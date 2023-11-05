/*
 * File: Motor_Control.h
 *
 * Code generated for Simulink model 'Motor_Control'.
 *
 * Model version                  : 1.21
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue Aug 22 23:45:15 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: NXP->Cortex-M4
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Motor_Control_h_
#define RTW_HEADER_Motor_Control_h_
#include <stddef.h>
#include <math.h>
#ifndef Motor_Control_COMMON_INCLUDES_
#define Motor_Control_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Motor_Control_COMMON_INCLUDES_ */

/* Model Code Variants */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#define Motor_Control_M                (rtM)

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real32_T UnitDelay7;                 /* '<S1>/Unit Delay7' */
  real32_T UnitDelay8;                 /* '<S1>/Unit Delay8' */
  real32_T Add1;                       /* '<S5>/Add1' */
  real32_T Add2;                       /* '<S5>/Add2' */
  real32_T Add1_d;                     /* '<S110>/Add1' */
  real32_T UnitDelay;                  /* '<S114>/Unit Delay' */
  real32_T Saturation;                 /* '<S50>/Saturation' */
  real32_T Saturation_j;               /* '<S100>/Saturation' */
  real32_T UnitDelay6;                 /* '<S1>/Unit Delay6' */
  real32_T UnitDelay5;                 /* '<S1>/Unit Delay5' */
  real32_T UnitDelay5_DSTATE;          /* '<S1>/Unit Delay5' */
  real32_T UnitDelay6_DSTATE;          /* '<S1>/Unit Delay6' */
  real32_T UnitDelay7_DSTATE;          /* '<S1>/Unit Delay7' */
  real32_T UnitDelay8_DSTATE;          /* '<S1>/Unit Delay8' */
  real32_T UnitDelay1_DSTATE;          /* '<S1>/Unit Delay1' */
  real32_T UnitDelay2_DSTATE;          /* '<S1>/Unit Delay2' */
  real32_T UnitDelay_DSTATE;           /* '<S112>/Unit Delay' */
  real32_T UnitDelay4_DSTATE;          /* '<S111>/Unit Delay4' */
  real32_T UnitDelay1_DSTATE_i;        /* '<S111>/Unit Delay1' */
  real32_T Integrator3_DSTATE;         /* '<S111>/Integrator3' */
  real32_T UnitDelay_DSTATE_c;         /* '<S111>/Unit Delay' */
  real32_T Integrator2_DSTATE;         /* '<S111>/Integrator2' */
  real32_T UnitDelay_DSTATE_b;         /* '<S114>/Unit Delay' */
  real32_T Integrator_DSTATE;          /* '<S149>/Integrator' */
  real32_T Integrator5_DSTATE;         /* '<S111>/Integrator5' */
  real32_T UnitDelay3_DSTATE;          /* '<S111>/Unit Delay3' */
  real32_T Integrator1_DSTATE;         /* '<S111>/Integrator1' */
  real32_T Integrator_DSTATE_m;        /* '<S43>/Integrator' */
  real32_T Integrator_DSTATE_mf;       /* '<S93>/Integrator' */
  uint8_T Integrator3_SYSTEM_ENABLE;   /* '<S111>/Integrator3' */
  uint8_T Integrator2_SYSTEM_ENABLE;   /* '<S111>/Integrator2' */
  uint8_T Integrator5_SYSTEM_ENABLE;   /* '<S111>/Integrator5' */
  uint8_T Integrator1_SYSTEM_ENABLE;   /* '<S111>/Integrator1' */
} DW;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T speed_ref;                  /* '<Root>/speed_ref' */
  real32_T abc[3];                     /* '<Root>/abc' */
  real32_T test_angle;                 /* '<Root>/test_angle' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real32_T Ta;                         /* '<Root>/Ta' */
  real32_T Tb;                         /* '<Root>/Tb' */
  real32_T Tc;                         /* '<Root>/Tc' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T *errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    struct {
      uint8_T TID[2];
    } TaskCounters;
  } Timing;
};

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern real32_T a_lbg_speed;           /* '<S1>/Unit Delay1' */
extern real32_T a_lbg_theta;           /* '<S1>/Unit Delay2' */

/* Model entry point functions */
extern void Motor_Control_initialize(void);
extern void Motor_Control_step(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Scope' : Unused code path elimination
 * Block '<S2>/Scope1' : Unused code path elimination
 * Block '<S1>/Constant' : Unused code path elimination
 * Block '<S1>/Constant1' : Unused code path elimination
 * Block '<S112>/Scope' : Unused code path elimination
 * Block '<S3>/Scope' : Unused code path elimination
 * Block '<S3>/Scope1' : Unused code path elimination
 * Block '<S3>/Scope2' : Unused code path elimination
 * Block '<S1>/Scope' : Unused code path elimination
 * Block '<S1>/Scope2' : Unused code path elimination
 * Block '<S1>/Scope6' : Unused code path elimination
 * Block '<S1>/Unit Delay3' : Unused code path elimination
 * Block '<S177>/Scope2' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('BLDC_G431/Motor_Control')    - opens subsystem BLDC_G431/Motor_Control
 * hilite_system('BLDC_G431/Motor_Control/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BLDC_G431'
 * '<S1>'   : 'BLDC_G431/Motor_Control'
 * '<S2>'   : 'BLDC_G431/Motor_Control/ACR'
 * '<S3>'   : 'BLDC_G431/Motor_Control/Luenberger'
 * '<S4>'   : 'BLDC_G431/Motor_Control/_clark'
 * '<S5>'   : 'BLDC_G431/Motor_Control/_inverse_park'
 * '<S6>'   : 'BLDC_G431/Motor_Control/_park'
 * '<S7>'   : 'BLDC_G431/Motor_Control/_svpwm'
 * '<S8>'   : 'BLDC_G431/Motor_Control/ACR/PID Controller1'
 * '<S9>'   : 'BLDC_G431/Motor_Control/ACR/PID Controller2'
 * '<S10>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Anti-windup'
 * '<S11>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/D Gain'
 * '<S12>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Filter'
 * '<S13>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Filter ICs'
 * '<S14>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/I Gain'
 * '<S15>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Ideal P Gain'
 * '<S16>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Ideal P Gain Fdbk'
 * '<S17>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Integrator'
 * '<S18>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Integrator ICs'
 * '<S19>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/N Copy'
 * '<S20>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/N Gain'
 * '<S21>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/P Copy'
 * '<S22>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Parallel P Gain'
 * '<S23>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Reset Signal'
 * '<S24>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Saturation'
 * '<S25>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Saturation Fdbk'
 * '<S26>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Sum'
 * '<S27>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Sum Fdbk'
 * '<S28>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tracking Mode'
 * '<S29>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tracking Mode Sum'
 * '<S30>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tsamp - Integral'
 * '<S31>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tsamp - Ngain'
 * '<S32>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/postSat Signal'
 * '<S33>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/preSat Signal'
 * '<S34>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Anti-windup/Disc. Clamping Parallel'
 * '<S35>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S36>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S37>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/D Gain/Disabled'
 * '<S38>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Filter/Disabled'
 * '<S39>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Filter ICs/Disabled'
 * '<S40>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/I Gain/Internal Parameters'
 * '<S41>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Ideal P Gain/Passthrough'
 * '<S42>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Ideal P Gain Fdbk/Disabled'
 * '<S43>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Integrator/Discrete'
 * '<S44>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Integrator ICs/Internal IC'
 * '<S45>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/N Copy/Disabled wSignal Specification'
 * '<S46>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/N Gain/Disabled'
 * '<S47>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/P Copy/Disabled'
 * '<S48>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Parallel P Gain/Internal Parameters'
 * '<S49>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Reset Signal/Disabled'
 * '<S50>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Saturation/Enabled'
 * '<S51>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Saturation Fdbk/Disabled'
 * '<S52>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Sum/Sum_PI'
 * '<S53>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Sum Fdbk/Disabled'
 * '<S54>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tracking Mode/Disabled'
 * '<S55>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tracking Mode Sum/Passthrough'
 * '<S56>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tsamp - Integral/Passthrough'
 * '<S57>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tsamp - Ngain/Passthrough'
 * '<S58>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/postSat Signal/Forward_Path'
 * '<S59>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/preSat Signal/Forward_Path'
 * '<S60>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Anti-windup'
 * '<S61>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/D Gain'
 * '<S62>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Filter'
 * '<S63>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Filter ICs'
 * '<S64>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/I Gain'
 * '<S65>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Ideal P Gain'
 * '<S66>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Ideal P Gain Fdbk'
 * '<S67>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Integrator'
 * '<S68>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Integrator ICs'
 * '<S69>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/N Copy'
 * '<S70>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/N Gain'
 * '<S71>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/P Copy'
 * '<S72>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Parallel P Gain'
 * '<S73>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Reset Signal'
 * '<S74>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Saturation'
 * '<S75>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Saturation Fdbk'
 * '<S76>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Sum'
 * '<S77>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Sum Fdbk'
 * '<S78>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tracking Mode'
 * '<S79>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tracking Mode Sum'
 * '<S80>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tsamp - Integral'
 * '<S81>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tsamp - Ngain'
 * '<S82>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/postSat Signal'
 * '<S83>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/preSat Signal'
 * '<S84>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Anti-windup/Disc. Clamping Parallel'
 * '<S85>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S86>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S87>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/D Gain/Disabled'
 * '<S88>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Filter/Disabled'
 * '<S89>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Filter ICs/Disabled'
 * '<S90>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/I Gain/Internal Parameters'
 * '<S91>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Ideal P Gain/Passthrough'
 * '<S92>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Ideal P Gain Fdbk/Disabled'
 * '<S93>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Integrator/Discrete'
 * '<S94>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Integrator ICs/Internal IC'
 * '<S95>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/N Copy/Disabled wSignal Specification'
 * '<S96>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/N Gain/Disabled'
 * '<S97>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/P Copy/Disabled'
 * '<S98>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Parallel P Gain/Internal Parameters'
 * '<S99>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Reset Signal/Disabled'
 * '<S100>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Saturation/Enabled'
 * '<S101>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Saturation Fdbk/Disabled'
 * '<S102>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Sum/Sum_PI'
 * '<S103>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Sum Fdbk/Disabled'
 * '<S104>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tracking Mode/Disabled'
 * '<S105>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tracking Mode Sum/Passthrough'
 * '<S106>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tsamp - Integral/Passthrough'
 * '<S107>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tsamp - Ngain/Passthrough'
 * '<S108>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/postSat Signal/Forward_Path'
 * '<S109>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/preSat Signal/Forward_Path'
 * '<S110>' : 'BLDC_G431/Motor_Control/Luenberger/LPF1'
 * '<S111>' : 'BLDC_G431/Motor_Control/Luenberger/Luenberger'
 * '<S112>' : 'BLDC_G431/Motor_Control/Luenberger/PLL'
 * '<S113>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl'
 * '<S114>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/SpeedToTheta'
 * '<S115>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/Subsystem1'
 * '<S116>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Anti-windup'
 * '<S117>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/D Gain'
 * '<S118>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Filter'
 * '<S119>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Filter ICs'
 * '<S120>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/I Gain'
 * '<S121>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Ideal P Gain'
 * '<S122>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Ideal P Gain Fdbk'
 * '<S123>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Integrator'
 * '<S124>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Integrator ICs'
 * '<S125>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/N Copy'
 * '<S126>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/N Gain'
 * '<S127>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/P Copy'
 * '<S128>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Parallel P Gain'
 * '<S129>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Reset Signal'
 * '<S130>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Saturation'
 * '<S131>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Saturation Fdbk'
 * '<S132>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Sum'
 * '<S133>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Sum Fdbk'
 * '<S134>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tracking Mode'
 * '<S135>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tracking Mode Sum'
 * '<S136>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tsamp - Integral'
 * '<S137>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tsamp - Ngain'
 * '<S138>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/postSat Signal'
 * '<S139>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/preSat Signal'
 * '<S140>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Anti-windup/Disc. Clamping Parallel'
 * '<S141>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S142>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S143>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/D Gain/Disabled'
 * '<S144>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Filter/Disabled'
 * '<S145>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Filter ICs/Disabled'
 * '<S146>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/I Gain/External Parameters'
 * '<S147>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Ideal P Gain/Passthrough'
 * '<S148>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Ideal P Gain Fdbk/Disabled'
 * '<S149>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Integrator/Discrete'
 * '<S150>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Integrator ICs/Internal IC'
 * '<S151>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/N Copy/Disabled wSignal Specification'
 * '<S152>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/N Gain/Disabled'
 * '<S153>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/P Copy/Disabled'
 * '<S154>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Parallel P Gain/External Parameters'
 * '<S155>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Reset Signal/Disabled'
 * '<S156>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Saturation/Enabled'
 * '<S157>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Saturation Fdbk/Disabled'
 * '<S158>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Sum/Sum_PI'
 * '<S159>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Sum Fdbk/Disabled'
 * '<S160>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tracking Mode/Disabled'
 * '<S161>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tracking Mode Sum/Passthrough'
 * '<S162>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tsamp - Integral/Passthrough'
 * '<S163>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tsamp - Ngain/Passthrough'
 * '<S164>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/postSat Signal/Forward_Path'
 * '<S165>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/preSat Signal/Forward_Path'
 * '<S166>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/SpeedToTheta/Angle_Limit'
 * '<S167>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/SpeedToTheta/Angle_Limit/If Action Subsystem1'
 * '<S168>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/SpeedToTheta/Angle_Limit/If Action Subsystem2'
 * '<S169>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/SpeedToTheta/Angle_Limit/If Action Subsystem3'
 * '<S170>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/Subsystem1/If Action Subsystem'
 * '<S171>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/Subsystem1/If Action Subsystem1'
 * '<S172>' : 'BLDC_G431/Motor_Control/_svpwm/N Sector  Caculate'
 * '<S173>' : 'BLDC_G431/Motor_Control/_svpwm/Subsystem'
 * '<S174>' : 'BLDC_G431/Motor_Control/_svpwm/Subsystem1'
 * '<S175>' : 'BLDC_G431/Motor_Control/_svpwm/Subsystem2'
 * '<S176>' : 'BLDC_G431/Motor_Control/_svpwm/T1 T2  Caculate'
 * '<S177>' : 'BLDC_G431/Motor_Control/_svpwm/Tcm calculate'
 * '<S178>' : 'BLDC_G431/Motor_Control/_svpwm/XYZ Caculate'
 */
#endif                                 /* RTW_HEADER_Motor_Control_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
