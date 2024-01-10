/*---------------------------------------
该文件为模块的配置文件，所有的输入信息都应
由该文件提供
----------------------------------------*/
#ifndef __PROTOCOL_CFG__H
#define __PROTOCOL_CFG__H

#include "stdlib.h"
#include "string.h"

#include "macro_defined.h"
#include "crc.h"
#include "fifo.h"
#include "ipc.h"
#include "list.h"
#include "fsm.h"
#include "debuglog.h"
#include "_assert.h"
#include "heap.h"


#undef NULL
#define NULL 0
#undef bool
#define bool	
#undef true
#define true	1
#undef false
#define false	0


#define PRO_FRAME_MIN_SIZE  8
#define	PRO_FRAME_MAX_SIZE	256
#define PRO_FIFO_SIZE PRO_FRAME_MAX_SIZE * (4)


#endif
