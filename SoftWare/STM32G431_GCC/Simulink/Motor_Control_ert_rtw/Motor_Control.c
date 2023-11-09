/*
 * File: Motor_Control.c
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

#include "Motor_Control.h"

/* Named constants for Chart: '<S1>/Chart' */
#define IN_AligStage                   ((uint8_T)1U)
#define IN_CLP                         ((uint8_T)2U)
#define IN_OPL                         ((uint8_T)3U)
#define NumBitsPerChar                 8U

/* Exported block signals */
real32_T a_reaid;                      /* '<S7>/Add1' */
real32_T a_realiq;                     /* '<S7>/Add2' */
real32_T a_lbg_speed;                  /* '<S1>/Unit Delay1' */
real32_T a_lbg_theta;                  /* '<S1>/Unit Delay2' */

/* Exported block parameters */
pid_t eleloop_param = {
  0.0F,
  0.0F,
  0.0F,
  0.6F
} ;                                    /* Variable: eleloop_param
                                        * Referenced by: '<S1>/Constant3'
                                        */

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;
extern real32_T rt_modf_snf(real32_T u0, real32_T u1);
static void open_loop_Enable(DW_open_loop *localDW);
static void open_loop(RT_MODEL * const rtM, real32_T rtu_opl_spd, real32_T
                      rtu_opl_time, real32_T rtu_opl_tor, real32_T
                      *rty_theta_fd1, real32_T *rty_iq_ref1, DW_open_loop
                      *localDW);
static void SCR_Enable(DW_SCR *localDW);
static void SCR(real32_T rtu_Speed_target, real32_T rtu_Obs_Speed, real32_T
                *rty_Iq_ref, const uint32_T *rtd_close_loop_ELAPS_T, DW_SCR
                *localDW);
static void close_loop_Enable(DW_close_loop *localDW);
static void close_loop(RT_MODEL * const rtM, real32_T rtu_obs_spd, real32_T
  rtu_obs_theta, real32_T rtu_speed_ref, real32_T *rty_theta_fd1, real32_T
  *rty_iq_ref1, DW_close_loop *localDW);
static void Luenberger_Init(void);
static void Luenberger_Enable(void);
static void Luenberger(void);
static void _inverse_park(void);
static void rate_scheduler(void);
static real_T rtGetNaN(void);
static real32_T rtGetNaNF(void);
extern real_T rtInf;
extern real_T rtMinusInf;
extern real_T rtNaN;
extern real32_T rtInfF;
extern real32_T rtMinusInfF;
extern real32_T rtNaNF;
static void rt_InitInfAndNaN(size_t realSize);
static boolean_T rtIsInf(real_T value);
static boolean_T rtIsInfF(real32_T value);
static boolean_T rtIsNaN(real_T value);
static boolean_T rtIsNaNF(real32_T value);
typedef struct {
  struct {
    uint32_T wordH;
    uint32_T wordL;
  } words;
} BigEndianIEEEDouble;

typedef struct {
  struct {
    uint32_T wordL;
    uint32_T wordH;
  } words;
} LittleEndianIEEEDouble;

typedef struct {
  union {
    real32_T wordLreal;
    uint32_T wordLuint;
  } wordL;
} IEEESingle;

real_T rtInf;
real_T rtMinusInf;
real_T rtNaN;
real32_T rtInfF;
real32_T rtMinusInfF;
real32_T rtNaNF;
static real_T rtGetInf(void);
static real32_T rtGetInfF(void);
static real_T rtGetMinusInf(void);
static real32_T rtGetMinusInfF(void);

/*
 * Initialize rtNaN needed by the generated code.
 * NaN is initialized as non-signaling. Assumes IEEE.
 */
static real_T rtGetNaN(void)
{
  size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
  real_T nan = 0.0;
  if (bitsPerReal == 32U) {
    nan = rtGetNaNF();
  } else {
    union {
      LittleEndianIEEEDouble bitVal;
      real_T fltVal;
    } tmpVal;

    tmpVal.bitVal.words.wordH = 0xFFF80000U;
    tmpVal.bitVal.words.wordL = 0x00000000U;
    nan = tmpVal.fltVal;
  }

  return nan;
}

/*
 * Initialize rtNaNF needed by the generated code.
 * NaN is initialized as non-signaling. Assumes IEEE.
 */
static real32_T rtGetNaNF(void)
{
  IEEESingle nanF = { { 0.0F } };

  nanF.wordL.wordLuint = 0xFFC00000U;
  return nanF.wordL.wordLreal;
}

/*
 * Initialize the rtInf, rtMinusInf, and rtNaN needed by the
 * generated code. NaN is initialized as non-signaling. Assumes IEEE.
 */
static void rt_InitInfAndNaN(size_t realSize)
{
  (void) (realSize);
  rtNaN = rtGetNaN();
  rtNaNF = rtGetNaNF();
  rtInf = rtGetInf();
  rtInfF = rtGetInfF();
  rtMinusInf = rtGetMinusInf();
  rtMinusInfF = rtGetMinusInfF();
}

/* Test if value is infinite */
static boolean_T rtIsInf(real_T value)
{
  return (boolean_T)((value==rtInf || value==rtMinusInf) ? 1U : 0U);
}

/* Test if single-precision value is infinite */
static boolean_T rtIsInfF(real32_T value)
{
  return (boolean_T)(((value)==rtInfF || (value)==rtMinusInfF) ? 1U : 0U);
}

/* Test if value is not a number */
static boolean_T rtIsNaN(real_T value)
{
  boolean_T result = (boolean_T) 0;
  size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
  if (bitsPerReal == 32U) {
    result = rtIsNaNF((real32_T)value);
  } else {
    union {
      LittleEndianIEEEDouble bitVal;
      real_T fltVal;
    } tmpVal;

    tmpVal.fltVal = value;
    result = (boolean_T)((tmpVal.bitVal.words.wordH & 0x7FF00000) == 0x7FF00000 &&
                         ( (tmpVal.bitVal.words.wordH & 0x000FFFFF) != 0 ||
                          (tmpVal.bitVal.words.wordL != 0) ));
  }

  return result;
}

/* Test if single-precision value is not a number */
static boolean_T rtIsNaNF(real32_T value)
{
  IEEESingle tmp;
  tmp.wordL.wordLreal = value;
  return (boolean_T)( (tmp.wordL.wordLuint & 0x7F800000) == 0x7F800000 &&
                     (tmp.wordL.wordLuint & 0x007FFFFF) != 0 );
}

/*
 * Initialize rtInf needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
static real_T rtGetInf(void)
{
  size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
  real_T inf = 0.0;
  if (bitsPerReal == 32U) {
    inf = rtGetInfF();
  } else {
    union {
      LittleEndianIEEEDouble bitVal;
      real_T fltVal;
    } tmpVal;

    tmpVal.bitVal.words.wordH = 0x7FF00000U;
    tmpVal.bitVal.words.wordL = 0x00000000U;
    inf = tmpVal.fltVal;
  }

  return inf;
}

/*
 * Initialize rtInfF needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
static real32_T rtGetInfF(void)
{
  IEEESingle infF;
  infF.wordL.wordLuint = 0x7F800000U;
  return infF.wordL.wordLreal;
}

/*
 * Initialize rtMinusInf needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
static real_T rtGetMinusInf(void)
{
  size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
  real_T minf = 0.0;
  if (bitsPerReal == 32U) {
    minf = rtGetMinusInfF();
  } else {
    union {
      LittleEndianIEEEDouble bitVal;
      real_T fltVal;
    } tmpVal;

    tmpVal.bitVal.words.wordH = 0xFFF00000U;
    tmpVal.bitVal.words.wordL = 0x00000000U;
    minf = tmpVal.fltVal;
  }

  return minf;
}

/*
 * Initialize rtMinusInfF needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
static real32_T rtGetMinusInfF(void)
{
  IEEESingle minfF;
  minfF.wordL.wordLuint = 0xFF800000U;
  return minfF.wordL.wordLreal;
}

/*
 *   This function updates active task flag for each subrate.
 * The function is called at model base rate, hence the
 * generated code self-manages all its subrates.
 */
static void rate_scheduler(void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (rtM->Timing.TaskCounters.TID[1])++;
  if ((rtM->Timing.TaskCounters.TID[1]) > 19) {/* Sample time: [0.002s, 0.0s] */
    rtM->Timing.TaskCounters.TID[1] = 0;
  }
}

real32_T rt_modf_snf(real32_T u0, real32_T u1)
{
  real32_T q;
  real32_T y;
  boolean_T yEq;
  y = u0;
  if (u1 == 0.0F) {
    if (u0 == 0.0F) {
      y = u1;
    }
  } else if (rtIsNaNF(u0) || rtIsNaNF(u1) || rtIsInfF(u0)) {
    y = (rtNaNF);
  } else if (u0 == 0.0F) {
    y = 0.0F / u1;
  } else if (rtIsInfF(u1)) {
    if ((u1 < 0.0F) != (u0 < 0.0F)) {
      y = u1;
    }
  } else {
    y = fmodf(u0, u1);
    yEq = (y == 0.0F);
    if ((!yEq) && (u1 > floorf(u1))) {
      q = fabsf(u0 / u1);
      yEq = !(fabsf(q - floorf(q + 0.5F)) > FLT_EPSILON * q);
    }

    if (yEq) {
      y = u1 * 0.0F;
    } else if ((u0 < 0.0F) != (u1 < 0.0F)) {
      y += u1;
    }
  }

  return y;
}

/* Enable for function-call system: '<S3>/open_loop' */
static void open_loop_Enable(DW_open_loop *localDW)
{
  localDW->open_loop_RESET_ELAPS_T = true;

  /* Enable for DiscreteIntegrator: '<S112>/Discrete-Time Integrator1' */
  localDW->DiscreteTimeIntegrator1_SYSTEM_ = 1U;

  /* Enable for DiscreteIntegrator: '<S112>/Discrete-Time Integrator2' */
  localDW->DiscreteTimeIntegrator2_SYSTEM_ = 1U;
}

/* Output and update for function-call system: '<S3>/open_loop' */
static void open_loop(RT_MODEL * const rtM, real32_T rtu_opl_spd, real32_T
                      rtu_opl_time, real32_T rtu_opl_tor, real32_T
                      *rty_theta_fd1, real32_T *rty_iq_ref1, DW_open_loop
                      *localDW)
{
  real32_T DiscreteTimeIntegrator1;
  real32_T DiscreteTimeIntegrator2;
  uint32_T open_loop_ELAPS_T;
  if (localDW->open_loop_RESET_ELAPS_T) {
    open_loop_ELAPS_T = 0U;
  } else {
    open_loop_ELAPS_T = rtM->Timing.clockTick0 - localDW->open_loop_PREV_T;
  }

  localDW->open_loop_PREV_T = rtM->Timing.clockTick0;
  localDW->open_loop_RESET_ELAPS_T = false;

  /* DiscreteIntegrator: '<S112>/Discrete-Time Integrator1' */
  if (localDW->DiscreteTimeIntegrator1_SYSTEM_ != 0) {
    /* DiscreteIntegrator: '<S112>/Discrete-Time Integrator1' */
    DiscreteTimeIntegrator1 = localDW->DiscreteTimeIntegrator1_DSTATE;
  } else {
    /* DiscreteIntegrator: '<S112>/Discrete-Time Integrator1' */
    DiscreteTimeIntegrator1 = 0.0001F * (real32_T)open_loop_ELAPS_T
      * localDW->DiscreteTimeIntegrator1_PREV_U +
      localDW->DiscreteTimeIntegrator1_DSTATE;
  }

  /* End of DiscreteIntegrator: '<S112>/Discrete-Time Integrator1' */

  /* Math: '<S112>/Mod2' incorporates:
   *  Constant: '<S112>/2pi2'
   */
  *rty_theta_fd1 = rt_modf_snf(DiscreteTimeIntegrator1, 6.28318548F);

  /* DiscreteIntegrator: '<S112>/Discrete-Time Integrator2' */
  if (localDW->DiscreteTimeIntegrator2_SYSTEM_ != 0) {
    /* DiscreteIntegrator: '<S112>/Discrete-Time Integrator2' */
    DiscreteTimeIntegrator2 = localDW->DiscreteTimeIntegrator2_DSTATE;
  } else {
    /* DiscreteIntegrator: '<S112>/Discrete-Time Integrator2' */
    DiscreteTimeIntegrator2 = 0.0001F * (real32_T)open_loop_ELAPS_T
      * localDW->DiscreteTimeIntegrator2_PREV_U +
      localDW->DiscreteTimeIntegrator2_DSTATE;
  }

  /* End of DiscreteIntegrator: '<S112>/Discrete-Time Integrator2' */

  /* Inport: '<S112>/opl_tor' */
  *rty_iq_ref1 = rtu_opl_tor;

  /* Update for DiscreteIntegrator: '<S112>/Discrete-Time Integrator1' incorporates:
   *  Gain: '<S112>/We1'
   */
  localDW->DiscreteTimeIntegrator1_SYSTEM_ = 0U;
  localDW->DiscreteTimeIntegrator1_DSTATE = DiscreteTimeIntegrator1;
  localDW->DiscreteTimeIntegrator1_PREV_U = 2.0F * DiscreteTimeIntegrator2;

  /* Update for DiscreteIntegrator: '<S112>/Discrete-Time Integrator2' incorporates:
   *  Constant: '<S112>/Constant'
   *  Gain: '<S112>/We'
   *  Product: '<S112>/Divide'
   *  Product: '<S112>/Product'
   */
  localDW->DiscreteTimeIntegrator2_SYSTEM_ = 0U;
  localDW->DiscreteTimeIntegrator2_DSTATE = DiscreteTimeIntegrator2;
  localDW->DiscreteTimeIntegrator2_PREV_U = 0.104719758F * rtu_opl_spd * (1.0F /
    rtu_opl_time);
}

/* Enable for atomic system: '<S111>/SCR' */
static void SCR_Enable(DW_SCR *localDW)
{
  /* Enable for DiscreteIntegrator: '<S114>/Discrete-Time Integrator' */
  localDW->DiscreteTimeIntegrator_SYSTEM_E = 1U;
}

/* Output and update for atomic system: '<S111>/SCR' */
static void SCR(real32_T rtu_Speed_target, real32_T rtu_Obs_Speed, real32_T
                *rty_Iq_ref, const uint32_T *rtd_close_loop_ELAPS_T, DW_SCR
                *localDW)
{
  real32_T DiscreteTimeIntegrator;
  real32_T rtb_Add_l;
  real32_T rtb_Switch_c;

  /* Sum: '<S113>/Sum' */
  rtb_Switch_c = rtu_Speed_target - rtu_Obs_Speed;

  /* DiscreteIntegrator: '<S114>/Discrete-Time Integrator' */
  if (localDW->DiscreteTimeIntegrator_SYSTEM_E != 0) {
    /* DiscreteIntegrator: '<S114>/Discrete-Time Integrator' */
    DiscreteTimeIntegrator = localDW->DiscreteTimeIntegrator_DSTATE;
  } else if (localDW->DiscreteTimeIntegrator_PrevRese != 0) {
    localDW->DiscreteTimeIntegrator_DSTATE = 0.0F;

    /* DiscreteIntegrator: '<S114>/Discrete-Time Integrator' */
    DiscreteTimeIntegrator = localDW->DiscreteTimeIntegrator_DSTATE;
  } else {
    /* DiscreteIntegrator: '<S114>/Discrete-Time Integrator' */
    DiscreteTimeIntegrator = 0.0001F * (real32_T)*rtd_close_loop_ELAPS_T
      * localDW->DiscreteTimeIntegrator_PREV_U +
      localDW->DiscreteTimeIntegrator_DSTATE;
  }

  /* End of DiscreteIntegrator: '<S114>/Discrete-Time Integrator' */

  /* Sum: '<S114>/Add' incorporates:
   *  Constant: '<S114>/Kp'
   *  Product: '<S114>/Product'
   */
  rtb_Add_l = 0.00867F * rtb_Switch_c + DiscreteTimeIntegrator;

  /* Saturate: '<S114>/Saturation' */
  if (rtb_Add_l > 6.0F) {
    *rty_Iq_ref = 6.0F;
  } else if (rtb_Add_l < -6.0F) {
    *rty_Iq_ref = -6.0F;
  } else {
    *rty_Iq_ref = rtb_Add_l;
  }

  /* End of Saturate: '<S114>/Saturation' */

  /* Update for DiscreteIntegrator: '<S114>/Discrete-Time Integrator' incorporates:
   *  Constant: '<S113>/Constant1'
   */
  localDW->DiscreteTimeIntegrator_SYSTEM_E = 0U;
  localDW->DiscreteTimeIntegrator_DSTATE = DiscreteTimeIntegrator;
  localDW->DiscreteTimeIntegrator_PrevRese = 0;

  /* Switch: '<S114>/Switch' incorporates:
   *  Constant: '<S114>/Cons'
   *  UnitDelay: '<S114>/Unit Delay3'
   */
  if (!localDW->UnitDelay3_DSTATE) {
    rtb_Switch_c = 0.0F;
  }

  /* End of Switch: '<S114>/Switch' */

  /* Update for DiscreteIntegrator: '<S114>/Discrete-Time Integrator' incorporates:
   *  Constant: '<S114>/Ki'
   *  Product: '<S114>/Product1'
   */
  localDW->DiscreteTimeIntegrator_PREV_U = 0.0633F * rtb_Switch_c;

  /* Update for UnitDelay: '<S114>/Unit Delay3' incorporates:
   *  RelationalOperator: '<S114>/Relational Operator1'
   */
  localDW->UnitDelay3_DSTATE = (rtb_Add_l == *rty_Iq_ref);
}

/* Enable for function-call system: '<S3>/close_loop' */
static void close_loop_Enable(DW_close_loop *localDW)
{
  localDW->close_loop_RESET_ELAPS_T = true;

  /* Enable for Atomic SubSystem: '<S111>/SCR' */
  SCR_Enable(&localDW->SCR_i);

  /* End of Enable for SubSystem: '<S111>/SCR' */
}

/* Output and update for function-call system: '<S3>/close_loop' */
static void close_loop(RT_MODEL * const rtM, real32_T rtu_obs_spd, real32_T
  rtu_obs_theta, real32_T rtu_speed_ref, real32_T *rty_theta_fd1, real32_T
  *rty_iq_ref1, DW_close_loop *localDW)
{
  uint32_T close_loop_ELAPS_T;
  if (localDW->close_loop_RESET_ELAPS_T) {
    close_loop_ELAPS_T = 0U;
  } else {
    close_loop_ELAPS_T = rtM->Timing.clockTick0 - localDW->close_loop_PREV_T;
  }

  localDW->close_loop_PREV_T = rtM->Timing.clockTick0;
  localDW->close_loop_RESET_ELAPS_T = false;

  /* Inport: '<S111>/obs_theta' */
  *rty_theta_fd1 = rtu_obs_theta;

  /* Outputs for Atomic SubSystem: '<S111>/SCR' */
  SCR(rtu_speed_ref, rtu_obs_spd, rty_iq_ref1, &close_loop_ELAPS_T,
      &localDW->SCR_i);

  /* End of Outputs for SubSystem: '<S111>/SCR' */
}

/* System initialize for atomic system: '<S1>/Luenberger' */
static void Luenberger_Init(void)
{
  /* InitializeConditions for DiscreteIntegrator: '<S154>/Integrator' */
  rtDW.Integrator_DSTATE = 1.0F;
}

/* Enable for atomic system: '<S1>/Luenberger' */
static void Luenberger_Enable(void)
{
  /* Enable for DiscreteIntegrator: '<S116>/Integrator3' */
  rtDW.Integrator3_SYSTEM_ENABLE = 1U;

  /* Enable for DiscreteIntegrator: '<S116>/Integrator2' */
  rtDW.Integrator2_SYSTEM_ENABLE = 1U;

  /* Enable for DiscreteIntegrator: '<S116>/Integrator5' */
  rtDW.Integrator5_SYSTEM_ENABLE = 1U;

  /* Enable for DiscreteIntegrator: '<S116>/Integrator1' */
  rtDW.Integrator1_SYSTEM_ENABLE = 1U;
}

/* Output and update for atomic system: '<S1>/Luenberger' */
static void Luenberger(void)
{
  int32_T tmp;
  real32_T Integrator2;
  real32_T Integrator3;
  real32_T rtb_IProdOut;
  real32_T rtb_Saturation;
  real32_T rtb_Sum1;
  real32_T rtb_Sum1_b;
  real32_T rtb_Sum_hj;
  real32_T rtb_Sum_j;
  real32_T rtb_UnitDelay;
  real32_T rtb_UnitDelay1;
  real32_T rtb_UnitDelay3;
  real32_T rtb_UnitDelay4;

  /* Sum: '<S115>/Add1' incorporates:
   *  Constant: '<S115>/Constant1'
   *  Gain: '<S4>/Gain5'
   *  Product: '<S115>/Divide1'
   *  Sum: '<S115>/Add'
   *  UnitDelay: '<S115>/Unit Delay'
   *  UnitDelay: '<S117>/Unit Delay'
   */
  rtDW.Add1_d += (4.77464819F * rtDW.UnitDelay_DSTATE - rtDW.Add1_d) * 0.3F;

  /* UnitDelay: '<S116>/Unit Delay4' */
  rtb_UnitDelay4 = rtDW.UnitDelay4_DSTATE;

  /* UnitDelay: '<S116>/Unit Delay1' */
  rtb_UnitDelay1 = rtDW.UnitDelay1_DSTATE_i;

  /* Sum: '<S116>/Sum1' */
  rtb_Sum1_b = rtb_UnitDelay1 - rtDW.UnitDelay8;

  /* DiscreteIntegrator: '<S116>/Integrator3' */
  if (rtDW.Integrator3_SYSTEM_ENABLE != 0) {
    /* DiscreteIntegrator: '<S116>/Integrator3' */
    Integrator3 = rtDW.Integrator3_DSTATE;
  } else {
    /* DiscreteIntegrator: '<S116>/Integrator3' incorporates:
     *  Gain: '<S116>/Gain11'
     *  Product: '<S116>/Product5'
     *  Sum: '<S116>/Add3'
     *  UnitDelay: '<S117>/Unit Delay'
     */
    Integrator3 = (rtb_UnitDelay4 * rtDW.UnitDelay_DSTATE + 14000.0F *
                   rtb_Sum1_b) * 0.0001F + rtDW.Integrator3_DSTATE;
  }

  /* End of DiscreteIntegrator: '<S116>/Integrator3' */

  /* UnitDelay: '<S116>/Unit Delay' */
  rtb_UnitDelay = rtDW.UnitDelay_DSTATE_c;

  /* Sum: '<S116>/Sum' */
  rtb_Sum_hj = rtb_UnitDelay - rtDW.UnitDelay7;

  /* DiscreteIntegrator: '<S116>/Integrator2' */
  if (rtDW.Integrator2_SYSTEM_ENABLE != 0) {
    /* DiscreteIntegrator: '<S116>/Integrator2' */
    Integrator2 = rtDW.Integrator2_DSTATE;
  } else {
    /* DiscreteIntegrator: '<S116>/Integrator2' incorporates:
     *  Gain: '<S116>/Gain12'
     *  Product: '<S116>/Product4'
     *  Sum: '<S116>/Add1'
     *  UnitDelay: '<S117>/Unit Delay'
     */
    Integrator2 = (14000.0F * rtb_Sum_hj - rtDW.UnitDelay_DSTATE * Integrator3) *
      0.0001F + rtDW.Integrator2_DSTATE;
  }

  /* End of DiscreteIntegrator: '<S116>/Integrator2' */

  /* UnitDelay: '<S119>/Unit Delay' */
  rtDW.UnitDelay = rtDW.UnitDelay_DSTATE_b;

  /* If: '<S120>/If' incorporates:
   *  Constant: '<S175>/Constant'
   *  Constant: '<S176>/Constant'
   *  SignalConversion generated from: '<S175>/Out1'
   *  SignalConversion generated from: '<S176>/Out1'
   *  UnitDelay: '<S117>/Unit Delay'
   */
  if (rtDW.UnitDelay_DSTATE > 0.0F) {
    /* Outputs for IfAction SubSystem: '<S120>/If Action Subsystem' incorporates:
     *  ActionPort: '<S175>/Action Port'
     */
    tmp = 1;

    /* End of Outputs for SubSystem: '<S120>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S120>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S176>/Action Port'
     */
    tmp = -1;

    /* End of Outputs for SubSystem: '<S120>/If Action Subsystem1' */
  }

  /* End of If: '<S120>/If' */

  /* Product: '<S120>/Product' incorporates:
   *  Product: '<S117>/Product1'
   *  Product: '<S117>/Product2'
   *  Sum: '<S117>/Sum'
   *  Trigonometry: '<S117>/Sin'
   *  Trigonometry: '<S117>/Sin1'
   */
  rtb_IProdOut = ((0.0F - cosf(rtDW.UnitDelay) * Integrator2) - Integrator3 *
                  sinf(rtDW.UnitDelay)) * (real32_T)tmp;

  /* Sum: '<S163>/Sum' incorporates:
   *  Constant: '<S117>/Kp'
   *  DiscreteIntegrator: '<S154>/Integrator'
   *  Product: '<S159>/PProd Out'
   */
  rtb_Sum_j = rtb_IProdOut * 70.7F + rtDW.Integrator_DSTATE;

  /* DeadZone: '<S147>/DeadZone' incorporates:
   *  Saturate: '<S161>/Saturation'
   */
  if (rtb_Sum_j > 3141.59277F) {
    rtb_UnitDelay3 = rtb_Sum_j - 3141.59277F;
    rtb_Saturation = 3141.59277F;
  } else {
    if (rtb_Sum_j >= -3141.59277F) {
      rtb_UnitDelay3 = 0.0F;
    } else {
      rtb_UnitDelay3 = rtb_Sum_j - -3141.59277F;
    }

    if (rtb_Sum_j < -3141.59277F) {
      rtb_Saturation = -3141.59277F;
    } else {
      rtb_Saturation = rtb_Sum_j;
    }
  }

  /* End of DeadZone: '<S147>/DeadZone' */

  /* Product: '<S151>/IProd Out' incorporates:
   *  Constant: '<S117>/Ki'
   */
  rtb_IProdOut *= 2500.0F;

  /* Sum: '<S119>/Sum1' incorporates:
   *  Gain: '<S119>/Ts'
   */
  rtb_Sum1 = 0.0001F * rtb_Saturation + rtDW.UnitDelay;

  /* DiscreteIntegrator: '<S116>/Integrator5' */
  if (rtDW.Integrator5_SYSTEM_ENABLE != 0) {
    /* DiscreteIntegrator: '<S116>/Integrator5' */
    rtb_UnitDelay4 = rtDW.Integrator5_DSTATE;
  } else {
    /* DiscreteIntegrator: '<S116>/Integrator5' incorporates:
     *  Gain: '<S116>/Gain'
     *  Gain: '<S116>/Gain2'
     *  Gain: '<S116>/Gain3'
     *  Gain: '<S116>/Gain4'
     *  Sum: '<S116>/Add'
     */
    rtb_UnitDelay4 = (((-2500.0F * rtb_UnitDelay4 + -4000.0F * rtb_Sum_hj) +
                       -500.0F * rtb_UnitDelay) + 2500.0F * rtDW.UnitDelay5) *
      0.0001F + rtDW.Integrator5_DSTATE;
  }

  /* End of DiscreteIntegrator: '<S116>/Integrator5' */

  /* DiscreteIntegrator: '<S116>/Integrator1' */
  if (rtDW.Integrator1_SYSTEM_ENABLE != 0) {
    /* DiscreteIntegrator: '<S116>/Integrator1' */
    rtb_UnitDelay1 = rtDW.Integrator1_DSTATE;
  } else {
    /* DiscreteIntegrator: '<S116>/Integrator1' incorporates:
     *  Gain: '<S116>/Gain10'
     *  Gain: '<S116>/Gain7'
     *  Gain: '<S116>/Gain8'
     *  Gain: '<S116>/Gain9'
     *  Sum: '<S116>/Add2'
     *  UnitDelay: '<S116>/Unit Delay3'
     */
    rtb_UnitDelay1 = (((2500.0F * rtDW.UnitDelay6 + -2500.0F *
                        rtDW.UnitDelay3_DSTATE) + -500.0F * rtb_UnitDelay1) +
                      -4000.0F * rtb_Sum1_b) * 0.0001F + rtDW.Integrator1_DSTATE;
  }

  /* End of DiscreteIntegrator: '<S116>/Integrator1' */

  /* Update for UnitDelay: '<S117>/Unit Delay' */
  rtDW.UnitDelay_DSTATE = rtb_Saturation;

  /* Update for UnitDelay: '<S116>/Unit Delay4' */
  rtDW.UnitDelay4_DSTATE = Integrator2;

  /* Update for UnitDelay: '<S116>/Unit Delay1' */
  rtDW.UnitDelay1_DSTATE_i = rtb_UnitDelay1;

  /* Update for DiscreteIntegrator: '<S116>/Integrator3' */
  rtDW.Integrator3_SYSTEM_ENABLE = 0U;
  rtDW.Integrator3_DSTATE = Integrator3;

  /* Update for UnitDelay: '<S116>/Unit Delay' */
  rtDW.UnitDelay_DSTATE_c = rtb_UnitDelay4;

  /* Update for DiscreteIntegrator: '<S116>/Integrator2' */
  rtDW.Integrator2_SYSTEM_ENABLE = 0U;
  rtDW.Integrator2_DSTATE = Integrator2;

  /* If: '<S171>/If1' */
  if (rtb_Sum1 > 6.28318548F) {
    /* Outputs for IfAction SubSystem: '<S171>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S173>/Action Port'
     */
    /* Update for UnitDelay: '<S119>/Unit Delay' incorporates:
     *  Constant: '<S173>/2pi'
     *  Sum: '<S173>/Subtract1'
     */
    rtDW.UnitDelay_DSTATE_b = rtb_Sum1 - 6.28318548F;

    /* End of Outputs for SubSystem: '<S171>/If Action Subsystem2' */
  } else if (rtb_Sum1 < 0.0F) {
    /* Outputs for IfAction SubSystem: '<S171>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S174>/Action Port'
     */
    /* Update for UnitDelay: '<S119>/Unit Delay' incorporates:
     *  Constant: '<S174>/Angle_offset'
     *  Sum: '<S174>/Subtract1'
     */
    rtDW.UnitDelay_DSTATE_b = rtb_Sum1 + 6.28318548F;

    /* End of Outputs for SubSystem: '<S171>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S171>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S172>/Action Port'
     */
    /* Update for UnitDelay: '<S119>/Unit Delay' incorporates:
     *  Inport: '<S172>/In1'
     */
    rtDW.UnitDelay_DSTATE_b = rtb_Sum1;

    /* End of Outputs for SubSystem: '<S171>/If Action Subsystem1' */
  }

  /* End of If: '<S171>/If1' */

  /* Signum: '<S145>/SignPreSat' */
  if (rtb_UnitDelay3 < 0.0F) {
    rtb_Sum1_b = -1.0F;
  } else if (rtb_UnitDelay3 > 0.0F) {
    rtb_Sum1_b = 1.0F;
  } else if (rtb_UnitDelay3 == 0.0F) {
    rtb_Sum1_b = 0.0F;
  } else {
    rtb_Sum1_b = (rtNaNF);
  }

  /* End of Signum: '<S145>/SignPreSat' */

  /* Signum: '<S145>/SignPreIntegrator' */
  if (rtb_IProdOut < 0.0F) {
    rtb_UnitDelay = -1.0F;
  } else if (rtb_IProdOut > 0.0F) {
    rtb_UnitDelay = 1.0F;
  } else if (rtb_IProdOut == 0.0F) {
    rtb_UnitDelay = 0.0F;
  } else {
    rtb_UnitDelay = (rtNaNF);
  }

  /* End of Signum: '<S145>/SignPreIntegrator' */

  /* Switch: '<S145>/Switch' incorporates:
   *  Constant: '<S145>/Constant1'
   *  DataTypeConversion: '<S145>/DataTypeConv1'
   *  DataTypeConversion: '<S145>/DataTypeConv2'
   *  Gain: '<S145>/ZeroGain'
   *  Logic: '<S145>/AND3'
   *  RelationalOperator: '<S145>/Equal1'
   *  RelationalOperator: '<S145>/NotEqual'
   */
  if ((0.0F * rtb_Sum_j != rtb_UnitDelay3) && ((int8_T)rtb_Sum1_b == (int8_T)
       rtb_UnitDelay)) {
    rtb_IProdOut = 0.0F;
  }

  /* End of Switch: '<S145>/Switch' */

  /* Update for DiscreteIntegrator: '<S154>/Integrator' */
  rtDW.Integrator_DSTATE += 0.0001F * rtb_IProdOut;

  /* Update for DiscreteIntegrator: '<S116>/Integrator5' */
  rtDW.Integrator5_SYSTEM_ENABLE = 0U;
  rtDW.Integrator5_DSTATE = rtb_UnitDelay4;

  /* Update for UnitDelay: '<S116>/Unit Delay3' */
  rtDW.UnitDelay3_DSTATE = Integrator3;

  /* Update for DiscreteIntegrator: '<S116>/Integrator1' */
  rtDW.Integrator1_SYSTEM_ENABLE = 0U;
  rtDW.Integrator1_DSTATE = rtb_UnitDelay1;
}

/* Output and update for atomic system: '<S1>/_inverse_park' */
static void _inverse_park(void)
{
  /* Sum: '<S6>/Add1' incorporates:
   *  Product: '<S6>/Product1'
   *  Product: '<S6>/Product2'
   */
  rtDW.Add1 = rtDW.Saturation_m * rtDW.UnitDelay6 - rtDW.Saturation_j *
    rtDW.UnitDelay5;

  /* Sum: '<S6>/Add2' incorporates:
   *  Product: '<S6>/Product3'
   *  Product: '<S6>/Product4'
   */
  rtDW.Add2 = rtDW.Saturation_m * rtDW.UnitDelay5 + rtDW.Saturation_j *
    rtDW.UnitDelay6;
}

/* Model step function */
void Motor_Control_step(void)
{
  real32_T rtb_Gain2_p;
  real32_T rtb_Gain5_m;
  real32_T rtb_IndexVector1;
  real32_T rtb_IntegralGain;
  real32_T rtb_Integrator_m;
  real32_T rtb_Sum_b;
  real32_T rtb_Switch_l;
  uint32_T elapsedTicks;

  /* Gain: '<S5>/Gain2' incorporates:
   *  Gain: '<S5>/Gain'
   *  Gain: '<S5>/Gain1'
   *  Inport: '<Root>/abc'
   *  Sum: '<S5>/Add'
   */
  rtb_Gain2_p = ((rtU.abc[0] - 0.5F * rtU.abc[1]) - 0.5F * rtU.abc[2]) *
    0.666666687F;

  /* Trigonometry: '<S1>/Cos2' incorporates:
   *  Inport: '<Root>/test_angle'
   */
  rtDW.UnitDelay6 = cosf(rtU.test_angle);

  /* Gain: '<S5>/Gain5' incorporates:
   *  Gain: '<S5>/Gain3'
   *  Gain: '<S5>/Gain4'
   *  Inport: '<Root>/abc'
   *  Sum: '<S5>/Add1'
   */
  rtb_Gain5_m = (0.866025388F * rtU.abc[1] - 0.866025388F * rtU.abc[2]) *
    0.666666687F;

  /* Trigonometry: '<S1>/Cos3' incorporates:
   *  Inport: '<Root>/test_angle'
   */
  rtDW.UnitDelay5 = sinf(rtU.test_angle);

  /* Sum: '<S7>/Add1' incorporates:
   *  Product: '<S7>/Product1'
   *  Product: '<S7>/Product2'
   */
  a_reaid = rtb_Gain2_p * rtDW.UnitDelay6 + rtb_Gain5_m * rtDW.UnitDelay5;

  /* Sum: '<S7>/Add2' incorporates:
   *  Product: '<S7>/Produc3t'
   *  Product: '<S7>/Product4'
   */
  a_realiq = rtDW.UnitDelay6 * rtb_Gain5_m - rtb_Gain2_p * rtDW.UnitDelay5;

  /* S-Function (fcgen): '<S1>/Function-Call Generator1' incorporates:
   *  SubSystem: '<S1>/ACR'
   */
  /* Sum: '<S53>/Sum' incorporates:
   *  Constant: '<S2>/Constant'
   *  DiscreteIntegrator: '<S44>/Integrator'
   *  Gain: '<S49>/Proportional Gain'
   *  Sum: '<S2>/Sum1'
   */
  rtb_IntegralGain = (0.0F - a_reaid) * 0.45F + rtDW.Integrator_DSTATE_m;

  /* DeadZone: '<S37>/DeadZone' */
  if (rtb_IntegralGain > 12.4707661F) {
    rtb_Integrator_m = rtb_IntegralGain - 12.4707661F;
  } else if (rtb_IntegralGain >= -12.4707661F) {
    rtb_Integrator_m = 0.0F;
  } else {
    rtb_Integrator_m = rtb_IntegralGain - -12.4707661F;
  }

  /* End of DeadZone: '<S37>/DeadZone' */

  /* Gain: '<S41>/Integral Gain' incorporates:
   *  Constant: '<S2>/Constant'
   *  Sum: '<S2>/Sum1'
   */
  rtb_Sum_b = (0.0F - a_reaid) * 450.0F;

  /* Signum: '<S35>/SignPreSat' */
  if (rtb_Integrator_m < 0.0F) {
    rtb_Switch_l = -1.0F;
  } else if (rtb_Integrator_m > 0.0F) {
    rtb_Switch_l = 1.0F;
  } else if (rtb_Integrator_m == 0.0F) {
    rtb_Switch_l = 0.0F;
  } else {
    rtb_Switch_l = (rtNaNF);
  }

  /* End of Signum: '<S35>/SignPreSat' */

  /* Signum: '<S35>/SignPreIntegrator' */
  if (rtb_Sum_b < 0.0F) {
    rtb_IndexVector1 = -1.0F;
  } else if (rtb_Sum_b > 0.0F) {
    rtb_IndexVector1 = 1.0F;
  } else if (rtb_Sum_b == 0.0F) {
    rtb_IndexVector1 = 0.0F;
  } else {
    rtb_IndexVector1 = (rtNaNF);
  }

  /* End of Signum: '<S35>/SignPreIntegrator' */

  /* Switch: '<S35>/Switch' incorporates:
   *  Constant: '<S35>/Constant1'
   *  DataTypeConversion: '<S35>/DataTypeConv1'
   *  DataTypeConversion: '<S35>/DataTypeConv2'
   *  Gain: '<S35>/ZeroGain'
   *  Logic: '<S35>/AND3'
   *  RelationalOperator: '<S35>/Equal1'
   *  RelationalOperator: '<S35>/NotEqual'
   */
  if ((0.0F * rtb_IntegralGain != rtb_Integrator_m) && ((int8_T)rtb_Switch_l ==
       (int8_T)rtb_IndexVector1)) {
    rtb_Switch_l = 0.0F;
  } else {
    rtb_Switch_l = rtb_Sum_b;
  }

  /* End of Switch: '<S35>/Switch' */

  /* Saturate: '<S51>/Saturation' */
  if (rtb_IntegralGain > 12.4707661F) {
    rtDW.Saturation_m = 12.4707661F;
  } else if (rtb_IntegralGain < -12.4707661F) {
    rtDW.Saturation_m = -12.4707661F;
  } else {
    rtDW.Saturation_m = rtb_IntegralGain;
  }

  /* End of Saturate: '<S51>/Saturation' */

  /* Sum: '<S2>/Sum7' incorporates:
   *  Constant: '<S1>/Constant3'
   */
  rtb_Integrator_m = eleloop_param.target - a_realiq;

  /* Gain: '<S91>/Integral Gain' */
  rtb_IntegralGain = 450.0F * rtb_Integrator_m;

  /* Sum: '<S103>/Sum' incorporates:
   *  DiscreteIntegrator: '<S94>/Integrator'
   *  Gain: '<S99>/Proportional Gain'
   */
  rtb_Sum_b = 0.45F * rtb_Integrator_m + rtDW.Integrator_DSTATE_mf;

  /* DeadZone: '<S87>/DeadZone' incorporates:
   *  Saturate: '<S101>/Saturation'
   */
  if (rtb_Sum_b > 12.4707661F) {
    rtb_Integrator_m = rtb_Sum_b - 12.4707661F;
    rtDW.Saturation_j = 12.4707661F;
  } else {
    if (rtb_Sum_b >= -12.4707661F) {
      rtb_Integrator_m = 0.0F;
    } else {
      rtb_Integrator_m = rtb_Sum_b - -12.4707661F;
    }

    if (rtb_Sum_b < -12.4707661F) {
      rtDW.Saturation_j = -12.4707661F;
    } else {
      rtDW.Saturation_j = rtb_Sum_b;
    }
  }

  /* End of DeadZone: '<S87>/DeadZone' */

  /* Update for DiscreteIntegrator: '<S44>/Integrator' */
  rtDW.Integrator_DSTATE_m += 0.0001F * rtb_Switch_l;

  /* Signum: '<S85>/SignPreSat' */
  if (rtb_Integrator_m < 0.0F) {
    rtb_Switch_l = -1.0F;
  } else if (rtb_Integrator_m > 0.0F) {
    rtb_Switch_l = 1.0F;
  } else if (rtb_Integrator_m == 0.0F) {
    rtb_Switch_l = 0.0F;
  } else {
    rtb_Switch_l = (rtNaNF);
  }

  /* End of Signum: '<S85>/SignPreSat' */

  /* Signum: '<S85>/SignPreIntegrator' */
  if (rtb_IntegralGain < 0.0F) {
    rtb_IndexVector1 = -1.0F;
  } else if (rtb_IntegralGain > 0.0F) {
    rtb_IndexVector1 = 1.0F;
  } else if (rtb_IntegralGain == 0.0F) {
    rtb_IndexVector1 = 0.0F;
  } else {
    rtb_IndexVector1 = (rtNaNF);
  }

  /* End of Signum: '<S85>/SignPreIntegrator' */

  /* Switch: '<S85>/Switch' incorporates:
   *  Constant: '<S85>/Constant1'
   *  DataTypeConversion: '<S85>/DataTypeConv1'
   *  DataTypeConversion: '<S85>/DataTypeConv2'
   *  Gain: '<S85>/ZeroGain'
   *  Logic: '<S85>/AND3'
   *  RelationalOperator: '<S85>/Equal1'
   *  RelationalOperator: '<S85>/NotEqual'
   */
  if ((0.0F * rtb_Sum_b != rtb_Integrator_m) && ((int8_T)rtb_Switch_l == (int8_T)
       rtb_IndexVector1)) {
    rtb_IntegralGain = 0.0F;
  }

  /* End of Switch: '<S85>/Switch' */

  /* Update for DiscreteIntegrator: '<S94>/Integrator' */
  rtDW.Integrator_DSTATE_mf += 0.0001F * rtb_IntegralGain;

  /* End of Outputs for S-Function (fcgen): '<S1>/Function-Call Generator1' */

  /* Outputs for Atomic SubSystem: '<S1>/_inverse_park' */
  _inverse_park();

  /* End of Outputs for SubSystem: '<S1>/_inverse_park' */

  /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
  /* Outputs for Atomic SubSystem: '<S8>/N Sector  Caculate' */
  /* Gain: '<S177>/Gain' incorporates:
   *  Gain: '<S183>/Gain'
   */
  rtb_Sum_b = 1.73205078F * rtDW.Add1;

  /* Sum: '<S177>/Add2' incorporates:
   *  Gain: '<S177>/Gain'
   *  Sum: '<S183>/Add2'
   */
  rtb_Integrator_m = (0.0F - rtDW.Add2) - rtb_Sum_b;

  /* Sum: '<S177>/Add1' incorporates:
   *  Gain: '<S177>/Gain'
   *  Sum: '<S183>/Add1'
   */
  rtb_Switch_l = rtb_Sum_b - rtDW.Add2;

  /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */

  /* Saturate: '<S177>/Saturation' incorporates:
   *  Gain: '<S177>/Gain1'
   *  Gain: '<S177>/Gain2'
   *  Sum: '<S177>/Add'
   *  Sum: '<S177>/Add1'
   *  Sum: '<S177>/Add2'
   *  Switch: '<S177>/Switch'
   *  Switch: '<S177>/Switch1'
   *  Switch: '<S177>/Switch2'
   */
  rtb_IntegralGain = (real32_T)((((rtb_Switch_l >= 0.0F) << 1) + (rtDW.Add2 >=
    0.0F)) + ((rtb_Integrator_m >= 0.0F) << 2));
  if (rtb_IntegralGain > 6.0F) {
    rtb_IntegralGain = 6.0F;
  } else if (rtb_IntegralGain < 1.0F) {
    rtb_IntegralGain = 1.0F;
  }

  /* End of Saturate: '<S177>/Saturation' */
  /* End of Outputs for SubSystem: '<S8>/N Sector  Caculate' */

  /* Outputs for Atomic SubSystem: '<S8>/T1 T2  Caculate' */
  /* MultiPortSwitch: '<S181>/Index Vector' incorporates:
   *  Gain: '<S181>/Gain'
   *  Gain: '<S181>/Gain1'
   *  Gain: '<S181>/Gain2'
   *  Gain: '<S183>/Gain1'
   *  Gain: '<S183>/Gain2'
   *  Product: '<S183>/Product'
   *  Product: '<S183>/Product1'
   *  Product: '<S183>/Product2'
   *  Sum: '<S183>/Add1'
   *  Sum: '<S183>/Add2'
   */
  switch ((int32_T)rtb_IntegralGain) {
   case 1:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_Integrator_m = rtb_Switch_l * -0.5F * 7.2168782E-6F;

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S183>/Gain1'
     *  Gain: '<S183>/Gain2'
     *  Product: '<S183>/Product1'
     *  Product: '<S183>/Product2'
     *  Sum: '<S183>/Add2'
     */
    rtb_Sum_b = ((0.0F - rtDW.Add2) - rtb_Sum_b) * -0.5F * 7.2168782E-6F;

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;

   case 2:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_Integrator_m = ((0.0F - rtDW.Add2) - rtb_Sum_b) * -0.5F * 7.2168782E-6F;

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S181>/Gain'
     *  Gain: '<S183>/Gain2'
     *  Product: '<S183>/Product'
     *  Product: '<S183>/Product1'
     *  Sum: '<S183>/Add2'
     */
    rtb_Sum_b = -(rtDW.Add2 * 7.2168782E-6F);

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;

   case 3:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_Integrator_m = -((rtb_Sum_b - rtDW.Add2) * -0.5F * 7.2168782E-6F);

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S181>/Gain2'
     *  Gain: '<S183>/Gain1'
     *  Product: '<S183>/Product'
     *  Product: '<S183>/Product2'
     *  Sum: '<S183>/Add1'
     */
    rtb_Sum_b = rtDW.Add2 * 7.2168782E-6F;

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;

   case 4:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_Integrator_m = -(rtDW.Add2 * 7.2168782E-6F);

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S181>/Gain'
     *  Gain: '<S183>/Gain1'
     *  Product: '<S183>/Product'
     *  Product: '<S183>/Product2'
     *  Sum: '<S183>/Add1'
     */
    rtb_Sum_b = (rtb_Sum_b - rtDW.Add2) * -0.5F * 7.2168782E-6F;

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;

   case 5:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_Integrator_m = rtDW.Add2 * 7.2168782E-6F;

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S181>/Gain1'
     *  Gain: '<S183>/Gain2'
     *  Product: '<S183>/Product'
     *  Product: '<S183>/Product1'
     *  Sum: '<S183>/Add2'
     */
    rtb_Sum_b = -(((0.0F - rtDW.Add2) - rtb_Sum_b) * -0.5F * 7.2168782E-6F);

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;

   default:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_Integrator_m = -(rtb_Integrator_m * -0.5F * 7.2168782E-6F);

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S181>/Gain1'
     *  Gain: '<S181>/Gain2'
     *  Gain: '<S183>/Gain1'
     *  Gain: '<S183>/Gain2'
     *  Product: '<S183>/Product1'
     *  Product: '<S183>/Product2'
     *  Sum: '<S183>/Add1'
     */
    rtb_Sum_b = -((rtb_Sum_b - rtDW.Add2) * -0.5F * 7.2168782E-6F);

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;
  }

  /* End of MultiPortSwitch: '<S181>/Index Vector' */

  /* Sum: '<S181>/Add' */
  rtb_Switch_l = rtb_Integrator_m + rtb_Sum_b;

  /* Switch: '<S181>/Switch' incorporates:
   *  Constant: '<S8>/Constant4'
   *  Product: '<S181>/Divide'
   *  Product: '<S181>/Divide1'
   *  Sum: '<S181>/Add1'
   *  Switch: '<S181>/Switch2'
   */
  if (!(0.0001F - rtb_Switch_l > 0.0F)) {
    rtb_Integrator_m = rtb_Integrator_m * 0.0001F / rtb_Switch_l;
    rtb_Sum_b *= 1.0F / rtb_Switch_l * 0.0001F;
  }

  /* End of Switch: '<S181>/Switch' */
  /* End of Outputs for SubSystem: '<S8>/T1 T2  Caculate' */

  /* Outputs for Atomic SubSystem: '<S8>/Tcm calculate' */
  /* Gain: '<S182>/Gain2' incorporates:
   *  Constant: '<S8>/Constant4'
   *  Sum: '<S182>/Add'
   */
  rtb_Switch_l = ((0.0001F - rtb_Integrator_m) - rtb_Sum_b) * 0.25F;

  /* Sum: '<S182>/Add1' incorporates:
   *  Gain: '<S182>/Gain'
   */
  rtb_Integrator_m = 0.5F * rtb_Integrator_m + rtb_Switch_l;

  /* MultiPortSwitch: '<S182>/Index Vector' incorporates:
   *  Gain: '<S182>/Gain1'
   *  Sum: '<S182>/Add2'
   */
  switch ((int32_T)rtb_IntegralGain) {
   case 1:
    rtb_IntegralGain = rtb_Integrator_m;

    /* MultiPortSwitch: '<S182>/Index Vector1' */
    rtb_IndexVector1 = rtb_Switch_l;

    /* MultiPortSwitch: '<S182>/Index Vector2' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_Switch_l = 0.5F * rtb_Sum_b + rtb_Integrator_m;
    break;

   case 2:
    rtb_IntegralGain = rtb_Switch_l;

    /* MultiPortSwitch: '<S182>/Index Vector1' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_IndexVector1 = 0.5F * rtb_Sum_b + rtb_Integrator_m;

    /* MultiPortSwitch: '<S182>/Index Vector2' */
    rtb_Switch_l = rtb_Integrator_m;
    break;

   case 3:
    rtb_IntegralGain = rtb_Switch_l;

    /* MultiPortSwitch: '<S182>/Index Vector1' */
    rtb_IndexVector1 = rtb_Integrator_m;

    /* MultiPortSwitch: '<S182>/Index Vector2' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_Switch_l = 0.5F * rtb_Sum_b + rtb_Integrator_m;
    break;

   case 4:
    rtb_IntegralGain = 0.5F * rtb_Sum_b + rtb_Integrator_m;

    /* MultiPortSwitch: '<S182>/Index Vector1' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_IndexVector1 = rtb_Integrator_m;
    break;

   case 5:
    rtb_IntegralGain = 0.5F * rtb_Sum_b + rtb_Integrator_m;

    /* MultiPortSwitch: '<S182>/Index Vector1' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_IndexVector1 = rtb_Switch_l;

    /* MultiPortSwitch: '<S182>/Index Vector2' */
    rtb_Switch_l = rtb_Integrator_m;
    break;

   default:
    rtb_IntegralGain = rtb_Integrator_m;

    /* MultiPortSwitch: '<S182>/Index Vector1' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_IndexVector1 = 0.5F * rtb_Sum_b + rtb_Integrator_m;
    break;
  }

  /* End of MultiPortSwitch: '<S182>/Index Vector' */
  /* End of Outputs for SubSystem: '<S8>/Tcm calculate' */

  /* Outport: '<Root>/Ta' incorporates:
   *  Constant: '<S178>/Constant'
   *  Gain: '<S178>/Gain1'
   *  Gain: '<S178>/Gain4'
   *  Sum: '<S178>/Add'
   */
  rtY.Ta = (0.0001F - 2.0F * rtb_IntegralGain) * 10000.0F;

  /* Outport: '<Root>/Tb' incorporates:
   *  Constant: '<S179>/Constant'
   *  Gain: '<S179>/Gain1'
   *  Gain: '<S179>/Gain4'
   *  Sum: '<S179>/Add'
   */
  rtY.Tb = (0.0001F - 2.0F * rtb_IndexVector1) * 10000.0F;

  /* Outport: '<Root>/Tc' incorporates:
   *  Constant: '<S180>/Constant'
   *  Gain: '<S180>/Gain1'
   *  Gain: '<S180>/Gain4'
   *  Sum: '<S180>/Add'
   */
  rtY.Tc = (0.0001F - 2.0F * rtb_Switch_l) * 10000.0F;

  /* UnitDelay: '<S1>/Unit Delay1' */
  a_lbg_speed = rtDW.UnitDelay1_DSTATE;

  /* S-Function (fcgen): '<S1>/Function-Call Generator2' incorporates:
   *  Chart: '<S1>/Chart'
   */
  /* Chart: '<S1>/Chart' incorporates:
   *  Constant: '<S1>/Constant13'
   *  Constant: '<S1>/Constant15'
   *  Constant: '<S1>/Constant8'
   *  Inport: '<Root>/speed_ref'
   *  Inport: '<Root>/test_angle'
   */
  elapsedTicks = rtM->Timing.clockTick0 - rtDW.previousTicks;
  rtDW.previousTicks = rtM->Timing.clockTick0;
  rtDW.temporalCounter_i1 += elapsedTicks;
  if (rtDW.is_active_c2_Motor_Control == 0U) {
    rtDW.is_active_c2_Motor_Control = 1U;
    rtDW.is_c2_Motor_Control = IN_AligStage;
    rtDW.temporalCounter_i1 = 0U;
  } else {
    switch (rtDW.is_c2_Motor_Control) {
     case IN_AligStage:
      if (rtDW.temporalCounter_i1 >= 500U) {
        rtDW.is_c2_Motor_Control = IN_OPL;
        rtDW.temporalCounter_i1 = 0U;

        /* Outputs for Function Call SubSystem: '<S3>/open_loop' */
        open_loop(rtM, 1200.0F, 2.0F, 3.0F, &rtb_Sum_b, &rtb_IntegralGain,
                  &rtDW.open_loop_k);

        /* End of Outputs for SubSystem: '<S3>/open_loop' */
      }
      break;

     case IN_CLP:
      /* Outputs for Function Call SubSystem: '<S3>/close_loop' */
      close_loop(rtM, a_lbg_speed, rtU.test_angle, rtU.speed_ref, &rtb_Sum_b,
                 &rtb_IntegralGain, &rtDW.close_loop_d);

      /* End of Outputs for SubSystem: '<S3>/close_loop' */
      break;

     default:
      /* case IN_OPL: */
      if ((rtDW.temporalCounter_i1 >= 20000U) && (a_lbg_speed > 800.0F)) {
        rtDW.is_c2_Motor_Control = IN_CLP;

        /* Outputs for Function Call SubSystem: '<S3>/close_loop' */
        close_loop(rtM, a_lbg_speed, rtU.test_angle, rtU.speed_ref, &rtb_Sum_b,
                   &rtb_IntegralGain, &rtDW.close_loop_d);

        /* End of Outputs for SubSystem: '<S3>/close_loop' */
      } else {
        /* Outputs for Function Call SubSystem: '<S3>/open_loop' */
        open_loop(rtM, 1200.0F, 2.0F, 3.0F, &rtb_Sum_b, &rtb_IntegralGain,
                  &rtDW.open_loop_k);

        /* End of Outputs for SubSystem: '<S3>/open_loop' */
      }
      break;
    }
  }

  /* End of Chart: '<S1>/Chart' */
  /* End of Outputs for S-Function (fcgen): '<S1>/Function-Call Generator2' */

  /* UnitDelay: '<S1>/Unit Delay5' */
  rtDW.UnitDelay5 = rtDW.UnitDelay5_DSTATE;

  /* UnitDelay: '<S1>/Unit Delay6' */
  rtDW.UnitDelay6 = rtDW.UnitDelay6_DSTATE;

  /* UnitDelay: '<S1>/Unit Delay7' */
  rtDW.UnitDelay7 = rtDW.UnitDelay7_DSTATE;

  /* UnitDelay: '<S1>/Unit Delay8' */
  rtDW.UnitDelay8 = rtDW.UnitDelay8_DSTATE;

  /* Outputs for Atomic SubSystem: '<S1>/Luenberger' */
  Luenberger();

  /* End of Outputs for SubSystem: '<S1>/Luenberger' */

  /* UnitDelay: '<S1>/Unit Delay2' */
  a_lbg_theta = rtDW.UnitDelay2_DSTATE;

  /* Update for UnitDelay: '<S1>/Unit Delay1' */
  rtDW.UnitDelay1_DSTATE = rtDW.Add1_d;

  /* Update for UnitDelay: '<S1>/Unit Delay5' */
  rtDW.UnitDelay5_DSTATE = rtDW.Add1;

  /* Update for UnitDelay: '<S1>/Unit Delay6' */
  rtDW.UnitDelay6_DSTATE = rtDW.Add2;

  /* Update for UnitDelay: '<S1>/Unit Delay7' */
  rtDW.UnitDelay7_DSTATE = rtb_Gain2_p;

  /* Update for UnitDelay: '<S1>/Unit Delay8' */
  rtDW.UnitDelay8_DSTATE = rtb_Gain5_m;

  /* Update for UnitDelay: '<S1>/Unit Delay2' */
  rtDW.UnitDelay2_DSTATE = rtDW.UnitDelay;

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The resolution of this integer timer is 0.0001, which is the step size
   * of the task. Size of "clockTick0" ensures timer will not overflow during the
   * application lifespan selected.
   */
  rtM->Timing.clockTick0++;
  rate_scheduler();
}

/* Model initialize function */
void Motor_Control_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* SystemInitialize for Atomic SubSystem: '<S1>/Luenberger' */
  Luenberger_Init();

  /* End of SystemInitialize for SubSystem: '<S1>/Luenberger' */

  /* Enable for S-Function (fcgen): '<S1>/Function-Call Generator2' incorporates:
   *  Chart: '<S1>/Chart'
   */
  /* Enable for Chart: '<S1>/Chart' incorporates:
   *  SubSystem: '<S3>/open_loop'
   */
  open_loop_Enable(&rtDW.open_loop_k);

  /* Enable for Chart: '<S1>/Chart' incorporates:
   *  SubSystem: '<S3>/close_loop'
   */
  close_loop_Enable(&rtDW.close_loop_d);
  rtDW.previousTicks = rtM->Timing.clockTick0;

  /* End of Enable for S-Function (fcgen): '<S1>/Function-Call Generator2' */

  /* Enable for Atomic SubSystem: '<S1>/Luenberger' */
  Luenberger_Enable();

  /* End of Enable for SubSystem: '<S1>/Luenberger' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
