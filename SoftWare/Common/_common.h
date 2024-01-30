#ifndef __COMMON__H
#define __COMMON__H

#define USE_IPC_ENABLE                 (0)
#define USE_FSM_ENABLE                 (0)
#define USE_GPIO_ENABLE                (0)
#define USE_ASSERT_ENABLE              (1)
#define USE_IQMATH_ENABLE              (1)
#define USE_METHOD_ENABLE              (1)
#define USE_DEBUGLOG_ENABLE            (1)
#define USE_ANALOGI2C_ENABLE           (0)
#define USE_DATASTRUCTURES_ENABLE      (1)

    #if (USE_IPC_ENABLE)
        #include "ipc.h"
    #endif

    #if (USE_FSM_ENABLE)
        #include "fsm.h"
    #endif

    #if (USE_GPIO_ENABLE)
        #include "gpio_pin.h"
    #endif

    #if (USE_ASSERT_ENABLE)
        #include "./_assert/_assert.h"
    #endif

    #if (USE_IQMATH_ENABLE)
        #include "IQmathLib.h"
    #endif

    #if (USE_METHOD_ENABLE)
        #include "method.h"
    #endif

    #if (USE_ANALOGI2C_ENABLE)
        #include "analog_i2c.h"
    #endif

    #if (USE_DEBUGLOG_ENABLE)
        #include "debuglog.h"
    #endif

    #if (USE_DATASTRUCTURES_ENABLE)
        #include "fifo.h"
        #include "heap.h"
    #endif

#endif
