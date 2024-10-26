#include "board.h"
#include "initmodule.h"
#include "taskmodule.h"
#include "debuglog.h"
#include "stm32g4xx_hal.h"
// #include "usart.h"
// #include "string.h"
// void _bsp_protransmit(unsigned char* pdata,unsigned short len)
// {
//     static unsigned char sg_uartsend_buf[125];
//     memcpy(sg_uartsend_buf,pdata,len);
//     HAL_UART_Transmit_DMA(&huart3,sg_uartsend_buf,len);
// }

#include "adc.h"
void board_deinit(void)
{

}
uint32_t Test_Vaule;
void baord_process(void)
{
    USER_DEBUG_NORMAL("%d\n",adc_getval());
}

board_task(baord_process)