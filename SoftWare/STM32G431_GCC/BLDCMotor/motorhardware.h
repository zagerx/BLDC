#ifndef __MOTORHARDWARE_CFG__H
#define __MOTORHARDWARE_CFG__H

#define AD_OFFSET                   2116.0f
#define RA_S                        0.01F
#define RB_S                        0.01F
#define RC_S                        0.01F
#define MOTOR_PAIR                  2.0f
#define BETA_                       5.7F

#define F_PWM                       10000.0f
#define T_PWM                       0.0001f
#define U_DC                        24.0f
#define T_UDC                       (float)(T_PWM/U_DC)

/*---CIRCLE_MAX_VAL = (2/3)*U_DC---*/
// #define CIRCLE_MAX_VAL               16.0f
#define CIRCLE_MAX_VAL               10.0f
#define D_MAX_VAL                    CIRCLE_MAX_VAL*0.95f
#define D_MIN_VAL                    -D_MAX_VAL
#define Q_MAX_VAL                    D_MAX_VAL
#define Q_MIN_VAL                    -Q_MAX_VAL
#endif
