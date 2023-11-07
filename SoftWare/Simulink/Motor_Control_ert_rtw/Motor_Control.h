/*
 * File: Motor_Control.h
 *
 * Code generated for Simulink model 'Motor_Control'.
 *
 * Model version                  : 1.28
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue Nov  7 17:39:38 2023
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
#include "rtwtypes.h"
#include <stddef.h>
#include <float.h>
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

#ifndef DEFINED_TYPEDEF_FOR_pid_t_
#define DEFINED_TYPEDEF_FOR_pid_t_

typedef struct {
  real32_T kp;
  real32_T ki;
  real32_T kd;
  real32_T target;
} pid_t;

#endif

/* Block signals and states (default storage) for system '<S3>/open_loop' */
typedef struct {
  real32_T DiscreteTimeIntegrator1_DSTATE;/* '<S112>/Discrete-Time Integrator1' */
  real32_T DiscreteTimeIntegrator2_DSTATE;/* '<S112>/Discrete-Time Integrator2' */
  real32_T DiscreteTimeIntegrator1_PREV_U;/* '<S112>/Discrete-Time Integrator1' */
  real32_T DiscreteTimeIntegrator2_PREV_U;/* '<S112>/Discrete-Time Integrator2' */
  uint32_T open_loop_PREV_T;           /* '<S3>/open_loop' */
  uint8_T DiscreteTimeIntegrator1_SYSTEM_;/* '<S112>/Discrete-Time Integrator1' */
  uint8_T DiscreteTimeIntegrator2_SYSTEM_;/* '<S112>/Discrete-Time Integrator2' */
  boolean_T open_loop_RESET_ELAPS_T;   /* '<S3>/open_loop' */
} DW_open_loop;

/* Block signals and states (default storage) for system '<S111>/SCR' */
typedef struct {
  real32_T DiscreteTimeIntegrator_DSTATE;/* '<S114>/Discrete-Time Integrator' */
  real32_T DiscreteTimeIntegrator_PREV_U;/* '<S114>/Discrete-Time Integrator' */
  int8_T DiscreteTimeIntegrator_PrevRese;/* '<S114>/Discrete-Time Integrator' */
  uint8_T DiscreteTimeIntegrator_SYSTEM_E;/* '<S114>/Discrete-Time Integrator' */
  boolean_T UnitDelay3_DSTATE;         /* '<S114>/Unit Delay3' */
} DW_SCR;

/* Block signals and states (default storage) for system '<S3>/close_loop' */
typedef struct {
  DW_SCR SCR_i;                        /* '<S111>/SCR' */
  uint32_T close_loop_PREV_T;          /* '<S3>/close_loop' */
  boolean_T close_loop_RESET_ELAPS_T;  /* '<S3>/close_loop' */
} DW_close_loop;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  DW_close_loop close_loop_d;          /* '<S3>/close_loop' */
  DW_open_loop open_loop_k;            /* '<S3>/open_loop' */
  real32_T UnitDelay7;                 /* '<S1>/Unit Delay7' */
  real32_T UnitDelay8;                 /* '<S1>/Unit Delay8' */
  real32_T Add1;                       /* '<S6>/Add1' */
  real32_T Add2;                       /* '<S6>/Add2' */
  real32_T Add1_d;                     /* '<S115>/Add1' */
  real32_T UnitDelay;                  /* '<S119>/Unit Delay' */
  real32_T Saturation_m;               /* '<S51>/Saturation' */
  real32_T Saturation_j;               /* '<S101>/Saturation' */
  real32_T UnitDelay6;                 /* '<S1>/Unit Delay6' */
  real32_T UnitDelay5;                 /* '<S1>/Unit Delay5' */
  real32_T UnitDelay1_DSTATE;          /* '<S1>/Unit Delay1' */
  real32_T UnitDelay5_DSTATE;          /* '<S1>/Unit Delay5' */
  real32_T UnitDelay6_DSTATE;          /* '<S1>/Unit Delay6' */
  real32_T UnitDelay7_DSTATE;          /* '<S1>/Unit Delay7' */
  real32_T UnitDelay8_DSTATE;          /* '<S1>/Unit Delay8' */
  real32_T UnitDelay2_DSTATE;          /* '<S1>/Unit Delay2' */
  real32_T UnitDelay_DSTATE;           /* '<S117>/Unit Delay' */
  real32_T UnitDelay4_DSTATE;          /* '<S116>/Unit Delay4' */
  real32_T UnitDelay1_DSTATE_i;        /* '<S116>/Unit Delay1' */
  real32_T Integrator3_DSTATE;         /* '<S116>/Integrator3' */
  real32_T UnitDelay_DSTATE_c;         /* '<S116>/Unit Delay' */
  real32_T Integrator2_DSTATE;         /* '<S116>/Integrator2' */
  real32_T UnitDelay_DSTATE_b;         /* '<S119>/Unit Delay' */
  real32_T Integrator_DSTATE;          /* '<S154>/Integrator' */
  real32_T Integrator5_DSTATE;         /* '<S116>/Integrator5' */
  real32_T UnitDelay3_DSTATE;          /* '<S116>/Unit Delay3' */
  real32_T Integrator1_DSTATE;         /* '<S116>/Integrator1' */
  real32_T Integrator_DSTATE_m;        /* '<S44>/Integrator' */
  real32_T Integrator_DSTATE_mf;       /* '<S94>/Integrator' */
  uint32_T temporalCounter_i1;         /* '<S1>/Chart' */
  uint32_T previousTicks;              /* '<S1>/Chart' */
  uint8_T Integrator3_SYSTEM_ENABLE;   /* '<S116>/Integrator3' */
  uint8_T Integrator2_SYSTEM_ENABLE;   /* '<S116>/Integrator2' */
  uint8_T Integrator5_SYSTEM_ENABLE;   /* '<S116>/Integrator5' */
  uint8_T Integrator1_SYSTEM_ENABLE;   /* '<S116>/Integrator1' */
  uint8_T is_active_c2_Motor_Control;  /* '<S1>/Chart' */
  uint8_T is_c2_Motor_Control;         /* '<S1>/Chart' */
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
    uint32_T clockTick0;
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
extern real32_T a_reaid;               /* '<S7>/Add1' */
extern real32_T a_realiq;              /* '<S7>/Add2' */
extern real32_T a_lbg_speed;           /* '<S1>/Unit Delay1' */
extern real32_T a_lbg_theta;           /* '<S1>/Unit Delay2' */

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern pid_t eleloop_param;            /* Variable: eleloop_param
                                        * Referenced by: '<S1>/Constant3'
                                        */

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
 * Block '<S111>/Constant' : Unused code path elimination
 * Block '<S111>/Data Type Conversion' : Unused code path elimination
 * Block '<S114>/Scope' : Unused code path elimination
 * Block '<S113>/Scope' : Unused code path elimination
 * Block '<S113>/Scope1' : Unused code path elimination
 * Block '<S111>/Scope' : Unused code path elimination
 * Block '<S111>/Scope1' : Unused code path elimination
 * Block '<S111>/Step' : Unused code path elimination
 * Block '<S112>/Scope' : Unused code path elimination
 * Block '<S112>/Scope1' : Unused code path elimination
 * Block '<S1>/Constant' : Unused code path elimination
 * Block '<S1>/Constant1' : Unused code path elimination
 * Block '<S117>/Scope' : Unused code path elimination
 * Block '<S4>/Scope' : Unused code path elimination
 * Block '<S4>/Scope1' : Unused code path elimination
 * Block '<S4>/Scope2' : Unused code path elimination
 * Block '<S1>/Scope' : Unused code path elimination
 * Block '<S1>/Scope2' : Unused code path elimination
 * Block '<S1>/Scope5' : Unused code path elimination
 * Block '<S1>/Scope6' : Unused code path elimination
 * Block '<S1>/Scope7' : Unused code path elimination
 * Block '<S1>/Scope9' : Unused code path elimination
 * Block '<S1>/Unit Delay13' : Unused code path elimination
 * Block '<S1>/Unit Delay14' : Unused code path elimination
 * Block '<S1>/Unit Delay3' : Unused code path elimination
 * Block '<S182>/Scope2' : Unused code path elimination
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
 * '<S3>'   : 'BLDC_G431/Motor_Control/Chart'
 * '<S4>'   : 'BLDC_G431/Motor_Control/Luenberger'
 * '<S5>'   : 'BLDC_G431/Motor_Control/_clark'
 * '<S6>'   : 'BLDC_G431/Motor_Control/_inverse_park'
 * '<S7>'   : 'BLDC_G431/Motor_Control/_park'
 * '<S8>'   : 'BLDC_G431/Motor_Control/_svpwm'
 * '<S9>'   : 'BLDC_G431/Motor_Control/ACR/PID Controller1'
 * '<S10>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2'
 * '<S11>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Anti-windup'
 * '<S12>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/D Gain'
 * '<S13>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Filter'
 * '<S14>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Filter ICs'
 * '<S15>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/I Gain'
 * '<S16>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Ideal P Gain'
 * '<S17>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Ideal P Gain Fdbk'
 * '<S18>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Integrator'
 * '<S19>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Integrator ICs'
 * '<S20>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/N Copy'
 * '<S21>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/N Gain'
 * '<S22>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/P Copy'
 * '<S23>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Parallel P Gain'
 * '<S24>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Reset Signal'
 * '<S25>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Saturation'
 * '<S26>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Saturation Fdbk'
 * '<S27>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Sum'
 * '<S28>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Sum Fdbk'
 * '<S29>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tracking Mode'
 * '<S30>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tracking Mode Sum'
 * '<S31>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tsamp - Integral'
 * '<S32>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tsamp - Ngain'
 * '<S33>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/postSat Signal'
 * '<S34>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/preSat Signal'
 * '<S35>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Anti-windup/Disc. Clamping Parallel'
 * '<S36>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S37>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S38>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/D Gain/Disabled'
 * '<S39>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Filter/Disabled'
 * '<S40>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Filter ICs/Disabled'
 * '<S41>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/I Gain/Internal Parameters'
 * '<S42>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Ideal P Gain/Passthrough'
 * '<S43>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Ideal P Gain Fdbk/Disabled'
 * '<S44>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Integrator/Discrete'
 * '<S45>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Integrator ICs/Internal IC'
 * '<S46>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/N Copy/Disabled wSignal Specification'
 * '<S47>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/N Gain/Disabled'
 * '<S48>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/P Copy/Disabled'
 * '<S49>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Parallel P Gain/Internal Parameters'
 * '<S50>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Reset Signal/Disabled'
 * '<S51>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Saturation/Enabled'
 * '<S52>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Saturation Fdbk/Disabled'
 * '<S53>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Sum/Sum_PI'
 * '<S54>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Sum Fdbk/Disabled'
 * '<S55>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tracking Mode/Disabled'
 * '<S56>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tracking Mode Sum/Passthrough'
 * '<S57>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tsamp - Integral/Passthrough'
 * '<S58>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/Tsamp - Ngain/Passthrough'
 * '<S59>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/postSat Signal/Forward_Path'
 * '<S60>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller1/preSat Signal/Forward_Path'
 * '<S61>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Anti-windup'
 * '<S62>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/D Gain'
 * '<S63>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Filter'
 * '<S64>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Filter ICs'
 * '<S65>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/I Gain'
 * '<S66>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Ideal P Gain'
 * '<S67>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Ideal P Gain Fdbk'
 * '<S68>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Integrator'
 * '<S69>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Integrator ICs'
 * '<S70>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/N Copy'
 * '<S71>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/N Gain'
 * '<S72>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/P Copy'
 * '<S73>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Parallel P Gain'
 * '<S74>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Reset Signal'
 * '<S75>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Saturation'
 * '<S76>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Saturation Fdbk'
 * '<S77>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Sum'
 * '<S78>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Sum Fdbk'
 * '<S79>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tracking Mode'
 * '<S80>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tracking Mode Sum'
 * '<S81>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tsamp - Integral'
 * '<S82>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tsamp - Ngain'
 * '<S83>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/postSat Signal'
 * '<S84>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/preSat Signal'
 * '<S85>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Anti-windup/Disc. Clamping Parallel'
 * '<S86>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S87>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S88>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/D Gain/Disabled'
 * '<S89>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Filter/Disabled'
 * '<S90>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Filter ICs/Disabled'
 * '<S91>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/I Gain/Internal Parameters'
 * '<S92>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Ideal P Gain/Passthrough'
 * '<S93>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Ideal P Gain Fdbk/Disabled'
 * '<S94>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Integrator/Discrete'
 * '<S95>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Integrator ICs/Internal IC'
 * '<S96>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/N Copy/Disabled wSignal Specification'
 * '<S97>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/N Gain/Disabled'
 * '<S98>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/P Copy/Disabled'
 * '<S99>'  : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Parallel P Gain/Internal Parameters'
 * '<S100>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Reset Signal/Disabled'
 * '<S101>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Saturation/Enabled'
 * '<S102>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Saturation Fdbk/Disabled'
 * '<S103>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Sum/Sum_PI'
 * '<S104>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Sum Fdbk/Disabled'
 * '<S105>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tracking Mode/Disabled'
 * '<S106>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tracking Mode Sum/Passthrough'
 * '<S107>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tsamp - Integral/Passthrough'
 * '<S108>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/Tsamp - Ngain/Passthrough'
 * '<S109>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/postSat Signal/Forward_Path'
 * '<S110>' : 'BLDC_G431/Motor_Control/ACR/PID Controller2/preSat Signal/Forward_Path'
 * '<S111>' : 'BLDC_G431/Motor_Control/Chart/close_loop'
 * '<S112>' : 'BLDC_G431/Motor_Control/Chart/open_loop'
 * '<S113>' : 'BLDC_G431/Motor_Control/Chart/close_loop/SCR'
 * '<S114>' : 'BLDC_G431/Motor_Control/Chart/close_loop/SCR/PI'
 * '<S115>' : 'BLDC_G431/Motor_Control/Luenberger/LPF1'
 * '<S116>' : 'BLDC_G431/Motor_Control/Luenberger/Luenberger'
 * '<S117>' : 'BLDC_G431/Motor_Control/Luenberger/PLL'
 * '<S118>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl'
 * '<S119>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/SpeedToTheta'
 * '<S120>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/Subsystem1'
 * '<S121>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Anti-windup'
 * '<S122>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/D Gain'
 * '<S123>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Filter'
 * '<S124>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Filter ICs'
 * '<S125>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/I Gain'
 * '<S126>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Ideal P Gain'
 * '<S127>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Ideal P Gain Fdbk'
 * '<S128>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Integrator'
 * '<S129>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Integrator ICs'
 * '<S130>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/N Copy'
 * '<S131>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/N Gain'
 * '<S132>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/P Copy'
 * '<S133>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Parallel P Gain'
 * '<S134>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Reset Signal'
 * '<S135>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Saturation'
 * '<S136>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Saturation Fdbk'
 * '<S137>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Sum'
 * '<S138>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Sum Fdbk'
 * '<S139>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tracking Mode'
 * '<S140>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tracking Mode Sum'
 * '<S141>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tsamp - Integral'
 * '<S142>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tsamp - Ngain'
 * '<S143>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/postSat Signal'
 * '<S144>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/preSat Signal'
 * '<S145>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Anti-windup/Disc. Clamping Parallel'
 * '<S146>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S147>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S148>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/D Gain/Disabled'
 * '<S149>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Filter/Disabled'
 * '<S150>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Filter ICs/Disabled'
 * '<S151>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/I Gain/External Parameters'
 * '<S152>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Ideal P Gain/Passthrough'
 * '<S153>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Ideal P Gain Fdbk/Disabled'
 * '<S154>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Integrator/Discrete'
 * '<S155>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Integrator ICs/Internal IC'
 * '<S156>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/N Copy/Disabled wSignal Specification'
 * '<S157>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/N Gain/Disabled'
 * '<S158>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/P Copy/Disabled'
 * '<S159>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Parallel P Gain/External Parameters'
 * '<S160>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Reset Signal/Disabled'
 * '<S161>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Saturation/Enabled'
 * '<S162>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Saturation Fdbk/Disabled'
 * '<S163>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Sum/Sum_PI'
 * '<S164>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Sum Fdbk/Disabled'
 * '<S165>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tracking Mode/Disabled'
 * '<S166>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tracking Mode Sum/Passthrough'
 * '<S167>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tsamp - Integral/Passthrough'
 * '<S168>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/Tsamp - Ngain/Passthrough'
 * '<S169>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/postSat Signal/Forward_Path'
 * '<S170>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/PIDTheta_Ctrl/preSat Signal/Forward_Path'
 * '<S171>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/SpeedToTheta/Angle_Limit'
 * '<S172>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/SpeedToTheta/Angle_Limit/If Action Subsystem1'
 * '<S173>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/SpeedToTheta/Angle_Limit/If Action Subsystem2'
 * '<S174>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/SpeedToTheta/Angle_Limit/If Action Subsystem3'
 * '<S175>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/Subsystem1/If Action Subsystem'
 * '<S176>' : 'BLDC_G431/Motor_Control/Luenberger/PLL/Subsystem1/If Action Subsystem1'
 * '<S177>' : 'BLDC_G431/Motor_Control/_svpwm/N Sector  Caculate'
 * '<S178>' : 'BLDC_G431/Motor_Control/_svpwm/Subsystem'
 * '<S179>' : 'BLDC_G431/Motor_Control/_svpwm/Subsystem1'
 * '<S180>' : 'BLDC_G431/Motor_Control/_svpwm/Subsystem2'
 * '<S181>' : 'BLDC_G431/Motor_Control/_svpwm/T1 T2  Caculate'
 * '<S182>' : 'BLDC_G431/Motor_Control/_svpwm/Tcm calculate'
 * '<S183>' : 'BLDC_G431/Motor_Control/_svpwm/XYZ Caculate'
 */
#endif                                 /* RTW_HEADER_Motor_Control_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
