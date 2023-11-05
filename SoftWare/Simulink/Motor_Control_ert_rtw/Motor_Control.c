/*
 * File: Motor_Control.c
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

#include "Motor_Control.h"
#define NumBitsPerChar                 8U

/* Exported block signals */
real32_T a_lbg_speed;                  /* '<S1>/Unit Delay1' */
real32_T a_lbg_theta;                  /* '<S1>/Unit Delay2' */

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;
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

/* System initialize for atomic system: '<S1>/Luenberger' */
static void Luenberger_Init(void)
{
  /* InitializeConditions for DiscreteIntegrator: '<S149>/Integrator' */
  rtDW.Integrator_DSTATE = 1.0F;
}

/* Enable for atomic system: '<S1>/Luenberger' */
static void Luenberger_Enable(void)
{
  /* Enable for DiscreteIntegrator: '<S111>/Integrator3' */
  rtDW.Integrator3_SYSTEM_ENABLE = 1U;

  /* Enable for DiscreteIntegrator: '<S111>/Integrator2' */
  rtDW.Integrator2_SYSTEM_ENABLE = 1U;

  /* Enable for DiscreteIntegrator: '<S111>/Integrator5' */
  rtDW.Integrator5_SYSTEM_ENABLE = 1U;

  /* Enable for DiscreteIntegrator: '<S111>/Integrator1' */
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

  /* Sum: '<S110>/Add1' incorporates:
   *  Constant: '<S110>/Constant1'
   *  Gain: '<S3>/Gain5'
   *  Product: '<S110>/Divide1'
   *  Sum: '<S110>/Add'
   *  UnitDelay: '<S110>/Unit Delay'
   *  UnitDelay: '<S112>/Unit Delay'
   */
  rtDW.Add1_d += (4.77464819F * rtDW.UnitDelay_DSTATE - rtDW.Add1_d) * 0.3F;

  /* UnitDelay: '<S111>/Unit Delay4' */
  rtb_UnitDelay4 = rtDW.UnitDelay4_DSTATE;

  /* UnitDelay: '<S111>/Unit Delay1' */
  rtb_UnitDelay1 = rtDW.UnitDelay1_DSTATE_i;

  /* Sum: '<S111>/Sum1' */
  rtb_Sum1_b = rtb_UnitDelay1 - rtDW.UnitDelay8;

  /* DiscreteIntegrator: '<S111>/Integrator3' */
  if (rtDW.Integrator3_SYSTEM_ENABLE != 0) {
    /* DiscreteIntegrator: '<S111>/Integrator3' */
    Integrator3 = rtDW.Integrator3_DSTATE;
  } else {
    /* DiscreteIntegrator: '<S111>/Integrator3' incorporates:
     *  Gain: '<S111>/Gain11'
     *  Product: '<S111>/Product5'
     *  Sum: '<S111>/Add3'
     *  UnitDelay: '<S112>/Unit Delay'
     */
    Integrator3 = (rtb_UnitDelay4 * rtDW.UnitDelay_DSTATE + 14000.0F *
                   rtb_Sum1_b) * 0.0001F + rtDW.Integrator3_DSTATE;
  }

  /* End of DiscreteIntegrator: '<S111>/Integrator3' */

  /* UnitDelay: '<S111>/Unit Delay' */
  rtb_UnitDelay = rtDW.UnitDelay_DSTATE_c;

  /* Sum: '<S111>/Sum' */
  rtb_Sum_hj = rtb_UnitDelay - rtDW.UnitDelay7;

  /* DiscreteIntegrator: '<S111>/Integrator2' */
  if (rtDW.Integrator2_SYSTEM_ENABLE != 0) {
    /* DiscreteIntegrator: '<S111>/Integrator2' */
    Integrator2 = rtDW.Integrator2_DSTATE;
  } else {
    /* DiscreteIntegrator: '<S111>/Integrator2' incorporates:
     *  Gain: '<S111>/Gain12'
     *  Product: '<S111>/Product4'
     *  Sum: '<S111>/Add1'
     *  UnitDelay: '<S112>/Unit Delay'
     */
    Integrator2 = (14000.0F * rtb_Sum_hj - rtDW.UnitDelay_DSTATE * Integrator3) *
      0.0001F + rtDW.Integrator2_DSTATE;
  }

  /* End of DiscreteIntegrator: '<S111>/Integrator2' */

  /* UnitDelay: '<S114>/Unit Delay' */
  rtDW.UnitDelay = rtDW.UnitDelay_DSTATE_b;

  /* If: '<S115>/If' incorporates:
   *  Constant: '<S170>/Constant'
   *  Constant: '<S171>/Constant'
   *  SignalConversion generated from: '<S170>/Out1'
   *  SignalConversion generated from: '<S171>/Out1'
   *  UnitDelay: '<S112>/Unit Delay'
   */
  if (rtDW.UnitDelay_DSTATE > 0.0F) {
    /* Outputs for IfAction SubSystem: '<S115>/If Action Subsystem' incorporates:
     *  ActionPort: '<S170>/Action Port'
     */
    tmp = 1;

    /* End of Outputs for SubSystem: '<S115>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S115>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S171>/Action Port'
     */
    tmp = -1;

    /* End of Outputs for SubSystem: '<S115>/If Action Subsystem1' */
  }

  /* End of If: '<S115>/If' */

  /* Product: '<S115>/Product' incorporates:
   *  Product: '<S112>/Product1'
   *  Product: '<S112>/Product2'
   *  Sum: '<S112>/Sum'
   *  Trigonometry: '<S112>/Sin'
   *  Trigonometry: '<S112>/Sin1'
   */
  rtb_IProdOut = ((0.0F - cosf(rtDW.UnitDelay) * Integrator2) - Integrator3 *
                  sinf(rtDW.UnitDelay)) * (real32_T)tmp;

  /* Sum: '<S158>/Sum' incorporates:
   *  Constant: '<S112>/Kp'
   *  DiscreteIntegrator: '<S149>/Integrator'
   *  Product: '<S154>/PProd Out'
   */
  rtb_Sum_j = rtb_IProdOut * 70.7F + rtDW.Integrator_DSTATE;

  /* DeadZone: '<S142>/DeadZone' incorporates:
   *  Saturate: '<S156>/Saturation'
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

  /* End of DeadZone: '<S142>/DeadZone' */

  /* Product: '<S146>/IProd Out' incorporates:
   *  Constant: '<S112>/Ki'
   */
  rtb_IProdOut *= 2500.0F;

  /* Sum: '<S114>/Sum1' incorporates:
   *  Gain: '<S114>/Ts'
   */
  rtb_Sum1 = 0.0001F * rtb_Saturation + rtDW.UnitDelay;

  /* DiscreteIntegrator: '<S111>/Integrator5' */
  if (rtDW.Integrator5_SYSTEM_ENABLE != 0) {
    /* DiscreteIntegrator: '<S111>/Integrator5' */
    rtb_UnitDelay4 = rtDW.Integrator5_DSTATE;
  } else {
    /* DiscreteIntegrator: '<S111>/Integrator5' incorporates:
     *  Gain: '<S111>/Gain'
     *  Gain: '<S111>/Gain2'
     *  Gain: '<S111>/Gain3'
     *  Gain: '<S111>/Gain4'
     *  Sum: '<S111>/Add'
     */
    rtb_UnitDelay4 = (((-2500.0F * rtb_UnitDelay4 + -4000.0F * rtb_Sum_hj) +
                       -500.0F * rtb_UnitDelay) + 2500.0F * rtDW.UnitDelay5) *
      0.0001F + rtDW.Integrator5_DSTATE;
  }

  /* End of DiscreteIntegrator: '<S111>/Integrator5' */

  /* DiscreteIntegrator: '<S111>/Integrator1' */
  if (rtDW.Integrator1_SYSTEM_ENABLE != 0) {
    /* DiscreteIntegrator: '<S111>/Integrator1' */
    rtb_UnitDelay1 = rtDW.Integrator1_DSTATE;
  } else {
    /* DiscreteIntegrator: '<S111>/Integrator1' incorporates:
     *  Gain: '<S111>/Gain10'
     *  Gain: '<S111>/Gain7'
     *  Gain: '<S111>/Gain8'
     *  Gain: '<S111>/Gain9'
     *  Sum: '<S111>/Add2'
     *  UnitDelay: '<S111>/Unit Delay3'
     */
    rtb_UnitDelay1 = (((2500.0F * rtDW.UnitDelay6 + -2500.0F *
                        rtDW.UnitDelay3_DSTATE) + -500.0F * rtb_UnitDelay1) +
                      -4000.0F * rtb_Sum1_b) * 0.0001F + rtDW.Integrator1_DSTATE;
  }

  /* End of DiscreteIntegrator: '<S111>/Integrator1' */

  /* Update for UnitDelay: '<S112>/Unit Delay' */
  rtDW.UnitDelay_DSTATE = rtb_Saturation;

  /* Update for UnitDelay: '<S111>/Unit Delay4' */
  rtDW.UnitDelay4_DSTATE = Integrator2;

  /* Update for UnitDelay: '<S111>/Unit Delay1' */
  rtDW.UnitDelay1_DSTATE_i = rtb_UnitDelay1;

  /* Update for DiscreteIntegrator: '<S111>/Integrator3' */
  rtDW.Integrator3_SYSTEM_ENABLE = 0U;
  rtDW.Integrator3_DSTATE = Integrator3;

  /* Update for UnitDelay: '<S111>/Unit Delay' */
  rtDW.UnitDelay_DSTATE_c = rtb_UnitDelay4;

  /* Update for DiscreteIntegrator: '<S111>/Integrator2' */
  rtDW.Integrator2_SYSTEM_ENABLE = 0U;
  rtDW.Integrator2_DSTATE = Integrator2;

  /* If: '<S166>/If1' */
  if (rtb_Sum1 > 6.28318548F) {
    /* Outputs for IfAction SubSystem: '<S166>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S168>/Action Port'
     */
    /* Update for UnitDelay: '<S114>/Unit Delay' incorporates:
     *  Constant: '<S168>/2pi'
     *  Sum: '<S168>/Subtract1'
     */
    rtDW.UnitDelay_DSTATE_b = rtb_Sum1 - 6.28318548F;

    /* End of Outputs for SubSystem: '<S166>/If Action Subsystem2' */
  } else if (rtb_Sum1 < 0.0F) {
    /* Outputs for IfAction SubSystem: '<S166>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S169>/Action Port'
     */
    /* Update for UnitDelay: '<S114>/Unit Delay' incorporates:
     *  Constant: '<S169>/Angle_offset'
     *  Sum: '<S169>/Subtract1'
     */
    rtDW.UnitDelay_DSTATE_b = rtb_Sum1 + 6.28318548F;

    /* End of Outputs for SubSystem: '<S166>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S166>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S167>/Action Port'
     */
    /* Update for UnitDelay: '<S114>/Unit Delay' incorporates:
     *  Inport: '<S167>/In1'
     */
    rtDW.UnitDelay_DSTATE_b = rtb_Sum1;

    /* End of Outputs for SubSystem: '<S166>/If Action Subsystem1' */
  }

  /* End of If: '<S166>/If1' */

  /* Signum: '<S140>/SignPreSat' */
  if (rtb_UnitDelay3 < 0.0F) {
    rtb_Sum1_b = -1.0F;
  } else if (rtb_UnitDelay3 > 0.0F) {
    rtb_Sum1_b = 1.0F;
  } else if (rtb_UnitDelay3 == 0.0F) {
    rtb_Sum1_b = 0.0F;
  } else {
    rtb_Sum1_b = (rtNaNF);
  }

  /* End of Signum: '<S140>/SignPreSat' */

  /* Signum: '<S140>/SignPreIntegrator' */
  if (rtb_IProdOut < 0.0F) {
    rtb_UnitDelay = -1.0F;
  } else if (rtb_IProdOut > 0.0F) {
    rtb_UnitDelay = 1.0F;
  } else if (rtb_IProdOut == 0.0F) {
    rtb_UnitDelay = 0.0F;
  } else {
    rtb_UnitDelay = (rtNaNF);
  }

  /* End of Signum: '<S140>/SignPreIntegrator' */

  /* Switch: '<S140>/Switch' incorporates:
   *  Constant: '<S140>/Constant1'
   *  DataTypeConversion: '<S140>/DataTypeConv1'
   *  DataTypeConversion: '<S140>/DataTypeConv2'
   *  Gain: '<S140>/ZeroGain'
   *  Logic: '<S140>/AND3'
   *  RelationalOperator: '<S140>/Equal1'
   *  RelationalOperator: '<S140>/NotEqual'
   */
  if ((0.0F * rtb_Sum_j != rtb_UnitDelay3) && ((int8_T)rtb_Sum1_b == (int8_T)
       rtb_UnitDelay)) {
    rtb_IProdOut = 0.0F;
  }

  /* End of Switch: '<S140>/Switch' */

  /* Update for DiscreteIntegrator: '<S149>/Integrator' */
  rtDW.Integrator_DSTATE += 0.0001F * rtb_IProdOut;

  /* Update for DiscreteIntegrator: '<S111>/Integrator5' */
  rtDW.Integrator5_SYSTEM_ENABLE = 0U;
  rtDW.Integrator5_DSTATE = rtb_UnitDelay4;

  /* Update for UnitDelay: '<S111>/Unit Delay3' */
  rtDW.UnitDelay3_DSTATE = Integrator3;

  /* Update for DiscreteIntegrator: '<S111>/Integrator1' */
  rtDW.Integrator1_SYSTEM_ENABLE = 0U;
  rtDW.Integrator1_DSTATE = rtb_UnitDelay1;
}

/* Output and update for atomic system: '<S1>/_inverse_park' */
static void _inverse_park(void)
{
  /* Sum: '<S5>/Add1' incorporates:
   *  Product: '<S5>/Product1'
   *  Product: '<S5>/Product2'
   */
  rtDW.Add1 = rtDW.Saturation * rtDW.UnitDelay6 - rtDW.Saturation_j *
    rtDW.UnitDelay5;

  /* Sum: '<S5>/Add2' incorporates:
   *  Product: '<S5>/Product3'
   *  Product: '<S5>/Product4'
   */
  rtDW.Add2 = rtDW.Saturation * rtDW.UnitDelay5 + rtDW.Saturation_j *
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
  real32_T rtb_Switch_a;

  /* Gain: '<S4>/Gain2' incorporates:
   *  Gain: '<S4>/Gain'
   *  Gain: '<S4>/Gain1'
   *  Inport: '<Root>/abc'
   *  Sum: '<S4>/Add'
   */
  rtb_Gain2_p = ((rtU.abc[0] - 0.5F * rtU.abc[1]) - 0.5F * rtU.abc[2]) *
    0.666666687F;

  /* Trigonometry: '<S1>/Cos2' incorporates:
   *  Inport: '<Root>/test_angle'
   */
  rtDW.UnitDelay6 = cosf(rtU.test_angle);

  /* Gain: '<S4>/Gain5' incorporates:
   *  Gain: '<S4>/Gain3'
   *  Gain: '<S4>/Gain4'
   *  Inport: '<Root>/abc'
   *  Sum: '<S4>/Add1'
   */
  rtb_Gain5_m = (0.866025388F * rtU.abc[1] - 0.866025388F * rtU.abc[2]) *
    0.666666687F;

  /* Trigonometry: '<S1>/Cos3' incorporates:
   *  Inport: '<Root>/test_angle'
   */
  rtDW.UnitDelay5 = sinf(rtU.test_angle);

  /* S-Function (fcgen): '<S1>/Function-Call Generator1' incorporates:
   *  SubSystem: '<S1>/ACR'
   */
  /* Sum: '<S2>/Sum1' incorporates:
   *  Constant: '<S2>/Constant'
   *  Product: '<S6>/Product1'
   *  Product: '<S6>/Product2'
   *  Sum: '<S6>/Add1'
   */
  rtb_Sum_b = 0.0F - (rtb_Gain2_p * rtDW.UnitDelay6 + rtb_Gain5_m *
                      rtDW.UnitDelay5);

  /* Sum: '<S52>/Sum' incorporates:
   *  DiscreteIntegrator: '<S43>/Integrator'
   *  Gain: '<S48>/Proportional Gain'
   */
  rtb_IntegralGain = 0.45F * rtb_Sum_b + rtDW.Integrator_DSTATE_m;

  /* DeadZone: '<S36>/DeadZone' */
  if (rtb_IntegralGain > 12.4707661F) {
    rtb_Integrator_m = rtb_IntegralGain - 12.4707661F;
  } else if (rtb_IntegralGain >= -12.4707661F) {
    rtb_Integrator_m = 0.0F;
  } else {
    rtb_Integrator_m = rtb_IntegralGain - -12.4707661F;
  }

  /* End of DeadZone: '<S36>/DeadZone' */

  /* Gain: '<S40>/Integral Gain' */
  rtb_Sum_b *= 450.0F;

  /* Signum: '<S34>/SignPreSat' */
  if (rtb_Integrator_m < 0.0F) {
    rtb_Switch_a = -1.0F;
  } else if (rtb_Integrator_m > 0.0F) {
    rtb_Switch_a = 1.0F;
  } else if (rtb_Integrator_m == 0.0F) {
    rtb_Switch_a = 0.0F;
  } else {
    rtb_Switch_a = (rtNaNF);
  }

  /* End of Signum: '<S34>/SignPreSat' */

  /* Signum: '<S34>/SignPreIntegrator' */
  if (rtb_Sum_b < 0.0F) {
    rtb_IndexVector1 = -1.0F;
  } else if (rtb_Sum_b > 0.0F) {
    rtb_IndexVector1 = 1.0F;
  } else if (rtb_Sum_b == 0.0F) {
    rtb_IndexVector1 = 0.0F;
  } else {
    rtb_IndexVector1 = (rtNaNF);
  }

  /* End of Signum: '<S34>/SignPreIntegrator' */

  /* Switch: '<S34>/Switch' incorporates:
   *  Constant: '<S34>/Constant1'
   *  DataTypeConversion: '<S34>/DataTypeConv1'
   *  DataTypeConversion: '<S34>/DataTypeConv2'
   *  Gain: '<S34>/ZeroGain'
   *  Logic: '<S34>/AND3'
   *  RelationalOperator: '<S34>/Equal1'
   *  RelationalOperator: '<S34>/NotEqual'
   */
  if ((0.0F * rtb_IntegralGain != rtb_Integrator_m) && ((int8_T)rtb_Switch_a ==
       (int8_T)rtb_IndexVector1)) {
    rtb_Switch_a = 0.0F;
  } else {
    rtb_Switch_a = rtb_Sum_b;
  }

  /* End of Switch: '<S34>/Switch' */

  /* Saturate: '<S50>/Saturation' */
  if (rtb_IntegralGain > 12.4707661F) {
    rtDW.Saturation = 12.4707661F;
  } else if (rtb_IntegralGain < -12.4707661F) {
    rtDW.Saturation = -12.4707661F;
  } else {
    rtDW.Saturation = rtb_IntegralGain;
  }

  /* End of Saturate: '<S50>/Saturation' */

  /* Sum: '<S2>/Sum7' incorporates:
   *  Constant: '<S1>/Constant2'
   *  Product: '<S6>/Produc3t'
   *  Product: '<S6>/Product4'
   *  Sum: '<S6>/Add2'
   */
  rtb_Integrator_m = 0.5F - (rtDW.UnitDelay6 * rtb_Gain5_m - rtb_Gain2_p *
    rtDW.UnitDelay5);

  /* Gain: '<S90>/Integral Gain' */
  rtb_IntegralGain = 450.0F * rtb_Integrator_m;

  /* Sum: '<S102>/Sum' incorporates:
   *  DiscreteIntegrator: '<S93>/Integrator'
   *  Gain: '<S98>/Proportional Gain'
   */
  rtb_Sum_b = 0.45F * rtb_Integrator_m + rtDW.Integrator_DSTATE_mf;

  /* DeadZone: '<S86>/DeadZone' incorporates:
   *  Saturate: '<S100>/Saturation'
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

  /* End of DeadZone: '<S86>/DeadZone' */

  /* Update for DiscreteIntegrator: '<S43>/Integrator' */
  rtDW.Integrator_DSTATE_m += 0.0001F * rtb_Switch_a;

  /* Signum: '<S84>/SignPreSat' */
  if (rtb_Integrator_m < 0.0F) {
    rtb_Switch_a = -1.0F;
  } else if (rtb_Integrator_m > 0.0F) {
    rtb_Switch_a = 1.0F;
  } else if (rtb_Integrator_m == 0.0F) {
    rtb_Switch_a = 0.0F;
  } else {
    rtb_Switch_a = (rtNaNF);
  }

  /* End of Signum: '<S84>/SignPreSat' */

  /* Signum: '<S84>/SignPreIntegrator' */
  if (rtb_IntegralGain < 0.0F) {
    rtb_IndexVector1 = -1.0F;
  } else if (rtb_IntegralGain > 0.0F) {
    rtb_IndexVector1 = 1.0F;
  } else if (rtb_IntegralGain == 0.0F) {
    rtb_IndexVector1 = 0.0F;
  } else {
    rtb_IndexVector1 = (rtNaNF);
  }

  /* End of Signum: '<S84>/SignPreIntegrator' */

  /* Switch: '<S84>/Switch' incorporates:
   *  Constant: '<S84>/Constant1'
   *  DataTypeConversion: '<S84>/DataTypeConv1'
   *  DataTypeConversion: '<S84>/DataTypeConv2'
   *  Gain: '<S84>/ZeroGain'
   *  Logic: '<S84>/AND3'
   *  RelationalOperator: '<S84>/Equal1'
   *  RelationalOperator: '<S84>/NotEqual'
   */
  if ((0.0F * rtb_Sum_b != rtb_Integrator_m) && ((int8_T)rtb_Switch_a == (int8_T)
       rtb_IndexVector1)) {
    rtb_IntegralGain = 0.0F;
  }

  /* End of Switch: '<S84>/Switch' */

  /* Update for DiscreteIntegrator: '<S93>/Integrator' */
  rtDW.Integrator_DSTATE_mf += 0.0001F * rtb_IntegralGain;

  /* End of Outputs for S-Function (fcgen): '<S1>/Function-Call Generator1' */

  /* Outputs for Atomic SubSystem: '<S1>/_inverse_park' */
  _inverse_park();

  /* End of Outputs for SubSystem: '<S1>/_inverse_park' */

  /* Outputs for Atomic SubSystem: '<S7>/XYZ Caculate' */
  /* Outputs for Atomic SubSystem: '<S7>/N Sector  Caculate' */
  /* Gain: '<S172>/Gain' incorporates:
   *  Gain: '<S178>/Gain'
   */
  rtb_Sum_b = 1.73205078F * rtDW.Add1;

  /* Sum: '<S172>/Add2' incorporates:
   *  Gain: '<S172>/Gain'
   *  Sum: '<S178>/Add2'
   */
  rtb_Integrator_m = (0.0F - rtDW.Add2) - rtb_Sum_b;

  /* Sum: '<S172>/Add1' incorporates:
   *  Gain: '<S172>/Gain'
   *  Sum: '<S178>/Add1'
   */
  rtb_Switch_a = rtb_Sum_b - rtDW.Add2;

  /* End of Outputs for SubSystem: '<S7>/XYZ Caculate' */

  /* Saturate: '<S172>/Saturation' incorporates:
   *  Gain: '<S172>/Gain1'
   *  Gain: '<S172>/Gain2'
   *  Sum: '<S172>/Add'
   *  Sum: '<S172>/Add1'
   *  Sum: '<S172>/Add2'
   *  Switch: '<S172>/Switch'
   *  Switch: '<S172>/Switch1'
   *  Switch: '<S172>/Switch2'
   */
  rtb_IntegralGain = (real32_T)((((rtb_Switch_a >= 0.0F) << 1) + (rtDW.Add2 >=
    0.0F)) + ((rtb_Integrator_m >= 0.0F) << 2));
  if (rtb_IntegralGain > 6.0F) {
    rtb_IntegralGain = 6.0F;
  } else if (rtb_IntegralGain < 1.0F) {
    rtb_IntegralGain = 1.0F;
  }

  /* End of Saturate: '<S172>/Saturation' */
  /* End of Outputs for SubSystem: '<S7>/N Sector  Caculate' */

  /* Outputs for Atomic SubSystem: '<S7>/T1 T2  Caculate' */
  /* MultiPortSwitch: '<S176>/Index Vector' incorporates:
   *  Gain: '<S176>/Gain'
   *  Gain: '<S176>/Gain1'
   *  Gain: '<S176>/Gain2'
   *  Gain: '<S178>/Gain1'
   *  Gain: '<S178>/Gain2'
   *  Product: '<S178>/Product'
   *  Product: '<S178>/Product1'
   *  Product: '<S178>/Product2'
   *  Sum: '<S178>/Add1'
   *  Sum: '<S178>/Add2'
   */
  switch ((int32_T)rtb_IntegralGain) {
   case 1:
    /* Outputs for Atomic SubSystem: '<S7>/XYZ Caculate' */
    rtb_Integrator_m = rtb_Switch_a * -0.5F * 7.2168782E-6F;

    /* MultiPortSwitch: '<S176>/Index Vector1' incorporates:
     *  Gain: '<S178>/Gain1'
     *  Gain: '<S178>/Gain2'
     *  Product: '<S178>/Product1'
     *  Product: '<S178>/Product2'
     *  Sum: '<S178>/Add2'
     */
    rtb_Sum_b = ((0.0F - rtDW.Add2) - rtb_Sum_b) * -0.5F * 7.2168782E-6F;

    /* End of Outputs for SubSystem: '<S7>/XYZ Caculate' */
    break;

   case 2:
    /* Outputs for Atomic SubSystem: '<S7>/XYZ Caculate' */
    rtb_Integrator_m = ((0.0F - rtDW.Add2) - rtb_Sum_b) * -0.5F * 7.2168782E-6F;

    /* MultiPortSwitch: '<S176>/Index Vector1' incorporates:
     *  Gain: '<S176>/Gain'
     *  Gain: '<S178>/Gain2'
     *  Product: '<S178>/Product'
     *  Product: '<S178>/Product1'
     *  Sum: '<S178>/Add2'
     */
    rtb_Sum_b = -(rtDW.Add2 * 7.2168782E-6F);

    /* End of Outputs for SubSystem: '<S7>/XYZ Caculate' */
    break;

   case 3:
    /* Outputs for Atomic SubSystem: '<S7>/XYZ Caculate' */
    rtb_Integrator_m = -((rtb_Sum_b - rtDW.Add2) * -0.5F * 7.2168782E-6F);

    /* MultiPortSwitch: '<S176>/Index Vector1' incorporates:
     *  Gain: '<S176>/Gain2'
     *  Gain: '<S178>/Gain1'
     *  Product: '<S178>/Product'
     *  Product: '<S178>/Product2'
     *  Sum: '<S178>/Add1'
     */
    rtb_Sum_b = rtDW.Add2 * 7.2168782E-6F;

    /* End of Outputs for SubSystem: '<S7>/XYZ Caculate' */
    break;

   case 4:
    /* Outputs for Atomic SubSystem: '<S7>/XYZ Caculate' */
    rtb_Integrator_m = -(rtDW.Add2 * 7.2168782E-6F);

    /* MultiPortSwitch: '<S176>/Index Vector1' incorporates:
     *  Gain: '<S176>/Gain'
     *  Gain: '<S178>/Gain1'
     *  Product: '<S178>/Product'
     *  Product: '<S178>/Product2'
     *  Sum: '<S178>/Add1'
     */
    rtb_Sum_b = (rtb_Sum_b - rtDW.Add2) * -0.5F * 7.2168782E-6F;

    /* End of Outputs for SubSystem: '<S7>/XYZ Caculate' */
    break;

   case 5:
    /* Outputs for Atomic SubSystem: '<S7>/XYZ Caculate' */
    rtb_Integrator_m = rtDW.Add2 * 7.2168782E-6F;

    /* MultiPortSwitch: '<S176>/Index Vector1' incorporates:
     *  Gain: '<S176>/Gain1'
     *  Gain: '<S178>/Gain2'
     *  Product: '<S178>/Product'
     *  Product: '<S178>/Product1'
     *  Sum: '<S178>/Add2'
     */
    rtb_Sum_b = -(((0.0F - rtDW.Add2) - rtb_Sum_b) * -0.5F * 7.2168782E-6F);

    /* End of Outputs for SubSystem: '<S7>/XYZ Caculate' */
    break;

   default:
    /* Outputs for Atomic SubSystem: '<S7>/XYZ Caculate' */
    rtb_Integrator_m = -(rtb_Integrator_m * -0.5F * 7.2168782E-6F);

    /* MultiPortSwitch: '<S176>/Index Vector1' incorporates:
     *  Gain: '<S176>/Gain1'
     *  Gain: '<S176>/Gain2'
     *  Gain: '<S178>/Gain1'
     *  Gain: '<S178>/Gain2'
     *  Product: '<S178>/Product1'
     *  Product: '<S178>/Product2'
     *  Sum: '<S178>/Add1'
     */
    rtb_Sum_b = -((rtb_Sum_b - rtDW.Add2) * -0.5F * 7.2168782E-6F);

    /* End of Outputs for SubSystem: '<S7>/XYZ Caculate' */
    break;
  }

  /* End of MultiPortSwitch: '<S176>/Index Vector' */

  /* Sum: '<S176>/Add' */
  rtb_Switch_a = rtb_Integrator_m + rtb_Sum_b;

  /* Switch: '<S176>/Switch' incorporates:
   *  Constant: '<S7>/Constant4'
   *  Product: '<S176>/Divide'
   *  Product: '<S176>/Divide1'
   *  Sum: '<S176>/Add1'
   *  Switch: '<S176>/Switch2'
   */
  if (!(0.0001F - rtb_Switch_a > 0.0F)) {
    rtb_Integrator_m = rtb_Integrator_m * 0.0001F / rtb_Switch_a;
    rtb_Sum_b *= 1.0F / rtb_Switch_a * 0.0001F;
  }

  /* End of Switch: '<S176>/Switch' */
  /* End of Outputs for SubSystem: '<S7>/T1 T2  Caculate' */

  /* Outputs for Atomic SubSystem: '<S7>/Tcm calculate' */
  /* Gain: '<S177>/Gain2' incorporates:
   *  Constant: '<S7>/Constant4'
   *  Sum: '<S177>/Add'
   */
  rtb_Switch_a = ((0.0001F - rtb_Integrator_m) - rtb_Sum_b) * 0.25F;

  /* Sum: '<S177>/Add1' incorporates:
   *  Gain: '<S177>/Gain'
   */
  rtb_Integrator_m = 0.5F * rtb_Integrator_m + rtb_Switch_a;

  /* MultiPortSwitch: '<S177>/Index Vector' incorporates:
   *  Gain: '<S177>/Gain1'
   *  Sum: '<S177>/Add2'
   */
  switch ((int32_T)rtb_IntegralGain) {
   case 1:
    rtb_IntegralGain = rtb_Integrator_m;

    /* MultiPortSwitch: '<S177>/Index Vector1' */
    rtb_IndexVector1 = rtb_Switch_a;

    /* MultiPortSwitch: '<S177>/Index Vector2' incorporates:
     *  Gain: '<S177>/Gain1'
     *  Sum: '<S177>/Add2'
     */
    rtb_Switch_a = 0.5F * rtb_Sum_b + rtb_Integrator_m;
    break;

   case 2:
    rtb_IntegralGain = rtb_Switch_a;

    /* MultiPortSwitch: '<S177>/Index Vector1' incorporates:
     *  Gain: '<S177>/Gain1'
     *  Sum: '<S177>/Add2'
     */
    rtb_IndexVector1 = 0.5F * rtb_Sum_b + rtb_Integrator_m;

    /* MultiPortSwitch: '<S177>/Index Vector2' */
    rtb_Switch_a = rtb_Integrator_m;
    break;

   case 3:
    rtb_IntegralGain = rtb_Switch_a;

    /* MultiPortSwitch: '<S177>/Index Vector1' */
    rtb_IndexVector1 = rtb_Integrator_m;

    /* MultiPortSwitch: '<S177>/Index Vector2' incorporates:
     *  Gain: '<S177>/Gain1'
     *  Sum: '<S177>/Add2'
     */
    rtb_Switch_a = 0.5F * rtb_Sum_b + rtb_Integrator_m;
    break;

   case 4:
    rtb_IntegralGain = 0.5F * rtb_Sum_b + rtb_Integrator_m;

    /* MultiPortSwitch: '<S177>/Index Vector1' incorporates:
     *  Gain: '<S177>/Gain1'
     *  Sum: '<S177>/Add2'
     */
    rtb_IndexVector1 = rtb_Integrator_m;
    break;

   case 5:
    rtb_IntegralGain = 0.5F * rtb_Sum_b + rtb_Integrator_m;

    /* MultiPortSwitch: '<S177>/Index Vector1' incorporates:
     *  Gain: '<S177>/Gain1'
     *  Sum: '<S177>/Add2'
     */
    rtb_IndexVector1 = rtb_Switch_a;

    /* MultiPortSwitch: '<S177>/Index Vector2' */
    rtb_Switch_a = rtb_Integrator_m;
    break;

   default:
    rtb_IntegralGain = rtb_Integrator_m;

    /* MultiPortSwitch: '<S177>/Index Vector1' incorporates:
     *  Gain: '<S177>/Gain1'
     *  Sum: '<S177>/Add2'
     */
    rtb_IndexVector1 = 0.5F * rtb_Sum_b + rtb_Integrator_m;
    break;
  }

  /* End of MultiPortSwitch: '<S177>/Index Vector' */
  /* End of Outputs for SubSystem: '<S7>/Tcm calculate' */

  /* Outport: '<Root>/Ta' incorporates:
   *  Constant: '<S173>/Constant'
   *  Gain: '<S173>/Gain1'
   *  Gain: '<S173>/Gain4'
   *  Sum: '<S173>/Add'
   */
  rtY.Ta = (0.0001F - 2.0F * rtb_IntegralGain) * 10000.0F;

  /* Outport: '<Root>/Tb' incorporates:
   *  Constant: '<S174>/Constant'
   *  Gain: '<S174>/Gain1'
   *  Gain: '<S174>/Gain4'
   *  Sum: '<S174>/Add'
   */
  rtY.Tb = (0.0001F - 2.0F * rtb_IndexVector1) * 10000.0F;

  /* Outport: '<Root>/Tc' incorporates:
   *  Constant: '<S175>/Constant'
   *  Gain: '<S175>/Gain1'
   *  Gain: '<S175>/Gain4'
   *  Sum: '<S175>/Add'
   */
  rtY.Tc = (0.0001F - 2.0F * rtb_Switch_a) * 10000.0F;

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

  /* UnitDelay: '<S1>/Unit Delay1' */
  a_lbg_speed = rtDW.UnitDelay1_DSTATE;

  /* UnitDelay: '<S1>/Unit Delay2' */
  a_lbg_theta = rtDW.UnitDelay2_DSTATE;

  /* Update for UnitDelay: '<S1>/Unit Delay5' */
  rtDW.UnitDelay5_DSTATE = rtDW.Add1;

  /* Update for UnitDelay: '<S1>/Unit Delay6' */
  rtDW.UnitDelay6_DSTATE = rtDW.Add2;

  /* Update for UnitDelay: '<S1>/Unit Delay7' */
  rtDW.UnitDelay7_DSTATE = rtb_Gain2_p;

  /* Update for UnitDelay: '<S1>/Unit Delay8' */
  rtDW.UnitDelay8_DSTATE = rtb_Gain5_m;

  /* Update for UnitDelay: '<S1>/Unit Delay1' */
  rtDW.UnitDelay1_DSTATE = rtDW.Add1_d;

  /* Update for UnitDelay: '<S1>/Unit Delay2' */
  rtDW.UnitDelay2_DSTATE = rtDW.UnitDelay;
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

  /* Enable for Atomic SubSystem: '<S1>/Luenberger' */
  Luenberger_Enable();

  /* End of Enable for SubSystem: '<S1>/Luenberger' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
