/****************************************************************************
Filename: mc_commend.h
Functions list:
Notes:
Copyright (c) 2022, Syrius Robotics
All rights reserved.
****************************************************************************/


#ifndef __MC_COMMEND__H
#define __MC_COMMEND__H

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"

enum{
    MODE_NORMAL = 0,
    MODE_SPEED,
    MODE_ENCODER,
    STATUS_START,
    STATUS_STOP,
    STATUS_RESET,
    SET_PIDTARGE
};

void motor_get_motorstop(void *obj,uint8_t *pdata,uint16_t datalen);
void motor_get_motorstart(void *obj,uint8_t *pdata,uint16_t datalen);
void motor_get_speedmode(void *obj,uint8_t *pdata,uint16_t datalen);
void motor_get_encodermode(void *obj,uint8_t *pdata,uint16_t datalen);
void motor_get_normolmode(void *obj,uint8_t *pdata,uint16_t datalen);
void motot_get_reset(void *obj,uint8_t *pdata,uint16_t datalen);
void motot_get_pidtarge(void *obj,uint8_t *pdata,uint16_t datalen);



#ifdef __cplusplus
}
#endif

#endif	/* __MC_COMMEND__H */
