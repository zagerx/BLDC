#ifndef __DAULT_DETECTION__H
#define __DAULT_DETECTION__H

typedef enum{
    NO_FAULT_CODE,
    FAULT_CODE_01,
    FAULT_CODE_02,
}EN_FAULTCODE;

typedef enum{
    NO_FAULT,
    NORMAL_FAULT,
}EN_FAULTSTA;

typedef struct _detection
{
    EN_FAULTCODE errcode;
    EN_FAULTSTA  status;
    float h_limit;
    float l_limit;
    float err_time;
    void (*errexcue)(void);
}detect_t;






#endif
