#ifndef _MOTOR_PROTOCOL__H
#define _MOTOR_PROTOCOL__H
#include "stdint.h"

enum{
    CMD_SET_STOP,
    CMD_SET_START,
    CMD_SET_NORMALMODE,
    CMD_SET_DEBUGMODE,
    CMD_SET_D_KP,
    CMD_SET_PIDTAR,
};
typedef void* (*pfunc)(char *,int32_t);
typedef struct {  
    char *cmd;  
    unsigned short cmd_index;
    pfunc pf_cmdexcue;
    char *res_cmd;
} commandmap_t; 
extern commandmap_t sg_commandmap[];



#define M_CMD                        (0x01)
#define M_SET_SPEED                  (0x0101)
#define M_SET_POS                    (0x0102)
#define M_SET_START                  (0x0103)
#define M_SET_STOP                   (0x0104)
#define M_SET_NormalM                (0x0105)
#define M_SET_DebugM                 (0x0106)
#define M_GET_MotorInfo              (0x0107)
#define M_GET_PCBAInfo               (0x0108)
#define M_SET_PIDParam               (0x0108)

#define S_CMD                        (0x02)
#define S_HeartP                     (0x0200)
#define S_SMInfo                     (0x0201)
#define S_PCBAInfo                   (0x0202)
#define S_MotorInfo                  (0x0203)


#endif
