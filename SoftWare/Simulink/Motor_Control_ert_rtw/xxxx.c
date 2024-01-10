/*
 * File: xxxx.c
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

#include "xxxx.h"

/* Include model header file for global data */
#include "Motor_Control.h"
#include "Motor_Control_private.h"

/* Output and update for atomic system: '<S1>/_svpwm' */
void s_svpwm(real32_T rtu_Valpha, real32_T rtu_Vbeta, real32_T *rty_Ta, real32_T
             *rty_Tb, real32_T *rty_Tc)
{
  real32_T rtb_Add_f;
  real32_T rtb_IndexVector;
  real32_T rtb_IndexVector1_f;
  real32_T rtb_IndexVector_p;
  real32_T rtb_Saturation_n;

  /* Outputs for Atomic SubSystem: '<S8>/N Sector  Caculate' */
  /* Switch: '<S177>/Switch' */
  *rty_Tc = (real32_T)(rtu_Vbeta >= 0.0F);

  /* Gain: '<S177>/Gain' */
  *rty_Tb = 1.73205078F * rtu_Valpha;

  /* Switch: '<S177>/Switch1' incorporates:
   *  Sum: '<S177>/Add1'
   */
  *rty_Ta = (real32_T)(*rty_Tb - rtu_Vbeta >= 0.0F);

  /* Switch: '<S177>/Switch2' incorporates:
   *  Sum: '<S177>/Add2'
   */
  *rty_Tb = (real32_T)((0.0F - rtu_Vbeta) - *rty_Tb >= 0.0F);

  /* Sum: '<S177>/Add' incorporates:
   *  Gain: '<S177>/Gain1'
   *  Gain: '<S177>/Gain2'
   */
  *rty_Tc = (2.0F * *rty_Ta + *rty_Tc) + 4.0F * *rty_Tb;

  /* Saturate: '<S177>/Saturation' */
  if (*rty_Tc > 6.0F) {
    rtb_Saturation_n = 6.0F;
  } else if (*rty_Tc < 1.0F) {
    rtb_Saturation_n = 1.0F;
  } else {
    rtb_Saturation_n = *rty_Tc;
  }

  /* End of Saturate: '<S177>/Saturation' */
  /* End of Outputs for SubSystem: '<S8>/N Sector  Caculate' */

  /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
  /* Gain: '<S183>/Gain' */
  *rty_Tc = 1.73205078F * rtu_Valpha;

  /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */

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
  switch ((int32_T)rtb_Saturation_n) {
   case 1:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_IndexVector_p = (*rty_Tc - rtu_Vbeta) * -0.5F * 7.2168782E-6F;

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S183>/Gain1'
     *  Gain: '<S183>/Gain2'
     *  Product: '<S183>/Product1'
     *  Product: '<S183>/Product2'
     *  Sum: '<S183>/Add1'
     *  Sum: '<S183>/Add2'
     */
    rtb_IndexVector1_f = ((0.0F - rtu_Vbeta) - *rty_Tc) * -0.5F * 7.2168782E-6F;

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;

   case 2:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_IndexVector_p = ((0.0F - rtu_Vbeta) - *rty_Tc) * -0.5F * 7.2168782E-6F;

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S181>/Gain'
     *  Gain: '<S183>/Gain2'
     *  Product: '<S183>/Product'
     *  Product: '<S183>/Product1'
     *  Sum: '<S183>/Add2'
     */
    rtb_IndexVector1_f = -(rtu_Vbeta * 7.2168782E-6F);

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;

   case 3:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_IndexVector_p = -((*rty_Tc - rtu_Vbeta) * -0.5F * 7.2168782E-6F);

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S181>/Gain2'
     *  Gain: '<S183>/Gain1'
     *  Product: '<S183>/Product'
     *  Product: '<S183>/Product2'
     *  Sum: '<S183>/Add1'
     */
    rtb_IndexVector1_f = rtu_Vbeta * 7.2168782E-6F;

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;

   case 4:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_IndexVector_p = -(rtu_Vbeta * 7.2168782E-6F);

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S181>/Gain'
     *  Gain: '<S183>/Gain1'
     *  Product: '<S183>/Product'
     *  Product: '<S183>/Product2'
     *  Sum: '<S183>/Add1'
     */
    rtb_IndexVector1_f = (*rty_Tc - rtu_Vbeta) * -0.5F * 7.2168782E-6F;

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;

   case 5:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_IndexVector_p = rtu_Vbeta * 7.2168782E-6F;

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S181>/Gain1'
     *  Gain: '<S183>/Gain2'
     *  Product: '<S183>/Product'
     *  Product: '<S183>/Product1'
     *  Sum: '<S183>/Add2'
     */
    rtb_IndexVector1_f = -(((0.0F - rtu_Vbeta) - *rty_Tc) * -0.5F *
      7.2168782E-6F);

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;

   default:
    /* Outputs for Atomic SubSystem: '<S8>/XYZ Caculate' */
    rtb_IndexVector_p = -(((0.0F - rtu_Vbeta) - *rty_Tc) * -0.5F * 7.2168782E-6F);

    /* MultiPortSwitch: '<S181>/Index Vector1' incorporates:
     *  Gain: '<S181>/Gain1'
     *  Gain: '<S181>/Gain2'
     *  Gain: '<S183>/Gain1'
     *  Gain: '<S183>/Gain2'
     *  Product: '<S183>/Product1'
     *  Product: '<S183>/Product2'
     *  Sum: '<S183>/Add1'
     *  Sum: '<S183>/Add2'
     */
    rtb_IndexVector1_f = -((*rty_Tc - rtu_Vbeta) * -0.5F * 7.2168782E-6F);

    /* End of Outputs for SubSystem: '<S8>/XYZ Caculate' */
    break;
  }

  /* End of MultiPortSwitch: '<S181>/Index Vector' */

  /* Sum: '<S181>/Add' */
  rtb_Add_f = rtb_IndexVector_p + rtb_IndexVector1_f;

  /* Sum: '<S181>/Add1' incorporates:
   *  Constant: '<S8>/Constant4'
   */
  *rty_Tc = 0.0001F - rtb_Add_f;

  /* Switch: '<S181>/Switch' incorporates:
   *  Constant: '<S8>/Constant4'
   *  Product: '<S181>/Divide'
   */
  if (!(*rty_Tc > 0.0F)) {
    rtb_IndexVector_p = rtb_IndexVector_p * 0.0001F / rtb_Add_f;
  }

  /* End of Switch: '<S181>/Switch' */

  /* Switch: '<S181>/Switch2' incorporates:
   *  Constant: '<S8>/Constant4'
   *  Product: '<S181>/Divide1'
   */
  if (!(*rty_Tc > 0.0F)) {
    rtb_IndexVector1_f *= 1.0F / rtb_Add_f * 0.0001F;
  }

  /* End of Switch: '<S181>/Switch2' */
  /* End of Outputs for SubSystem: '<S8>/T1 T2  Caculate' */

  /* Outputs for Atomic SubSystem: '<S8>/Tcm calculate' */
  /* Gain: '<S182>/Gain2' incorporates:
   *  Constant: '<S8>/Constant4'
   *  Sum: '<S182>/Add'
   */
  rtb_Add_f = ((0.0001F - rtb_IndexVector_p) - rtb_IndexVector1_f) * 0.25F;

  /* Sum: '<S182>/Add1' incorporates:
   *  Gain: '<S182>/Gain'
   */
  rtb_IndexVector_p = 0.5F * rtb_IndexVector_p + rtb_Add_f;

  /* MultiPortSwitch: '<S182>/Index Vector' incorporates:
   *  Gain: '<S182>/Gain1'
   *  Sum: '<S182>/Add2'
   */
  switch ((int32_T)rtb_Saturation_n) {
   case 1:
    rtb_IndexVector = rtb_IndexVector_p;

    /* MultiPortSwitch: '<S182>/Index Vector1' */
    rtb_Saturation_n = rtb_Add_f;

    /* MultiPortSwitch: '<S182>/Index Vector2' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_Add_f = 0.5F * rtb_IndexVector1_f + rtb_IndexVector_p;
    break;

   case 2:
    rtb_IndexVector = rtb_Add_f;

    /* MultiPortSwitch: '<S182>/Index Vector1' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_Saturation_n = 0.5F * rtb_IndexVector1_f + rtb_IndexVector_p;

    /* MultiPortSwitch: '<S182>/Index Vector2' */
    rtb_Add_f = rtb_IndexVector_p;
    break;

   case 3:
    rtb_IndexVector = rtb_Add_f;

    /* MultiPortSwitch: '<S182>/Index Vector1' */
    rtb_Saturation_n = rtb_IndexVector_p;

    /* MultiPortSwitch: '<S182>/Index Vector2' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_Add_f = 0.5F * rtb_IndexVector1_f + rtb_IndexVector_p;
    break;

   case 4:
    rtb_IndexVector = 0.5F * rtb_IndexVector1_f + rtb_IndexVector_p;

    /* MultiPortSwitch: '<S182>/Index Vector1' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_Saturation_n = rtb_IndexVector_p;
    break;

   case 5:
    rtb_IndexVector = 0.5F * rtb_IndexVector1_f + rtb_IndexVector_p;

    /* MultiPortSwitch: '<S182>/Index Vector1' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_Saturation_n = rtb_Add_f;

    /* MultiPortSwitch: '<S182>/Index Vector2' */
    rtb_Add_f = rtb_IndexVector_p;
    break;

   default:
    rtb_IndexVector = rtb_IndexVector_p;

    /* MultiPortSwitch: '<S182>/Index Vector1' incorporates:
     *  Gain: '<S182>/Gain1'
     *  Sum: '<S182>/Add2'
     */
    rtb_Saturation_n = 0.5F * rtb_IndexVector1_f + rtb_IndexVector_p;
    break;
  }

  /* End of MultiPortSwitch: '<S182>/Index Vector' */
  /* End of Outputs for SubSystem: '<S8>/Tcm calculate' */

  /* Gain: '<S178>/Gain4' incorporates:
   *  Constant: '<S178>/Constant'
   *  Gain: '<S178>/Gain1'
   *  Sum: '<S178>/Add'
   */
  *rty_Ta = (0.0001F - 2.0F * rtb_IndexVector) * 10000.0F;

  /* Gain: '<S179>/Gain4' incorporates:
   *  Constant: '<S179>/Constant'
   *  Gain: '<S179>/Gain1'
   *  Sum: '<S179>/Add'
   */
  *rty_Tb = (0.0001F - 2.0F * rtb_Saturation_n) * 10000.0F;

  /* Gain: '<S180>/Gain4' incorporates:
   *  Constant: '<S180>/Constant'
   *  Gain: '<S180>/Gain1'
   *  Sum: '<S180>/Add'
   */
  *rty_Tc = (0.0001F - 2.0F * rtb_Add_f) * 10000.0F;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
