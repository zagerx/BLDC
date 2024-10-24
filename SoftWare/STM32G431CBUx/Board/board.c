#include "board.h"
#include "initmodule.h"
#include "debuglog.h"

// #include "usart.h"
// #include "string.h"
// void _bsp_protransmit(unsigned char* pdata,unsigned short len)
// {
//     static unsigned char sg_uartsend_buf[125];
//     memcpy(sg_uartsend_buf,pdata,len);
//     HAL_UART_Transmit_DMA(&huart3,sg_uartsend_buf,len);
// }


void user_board_init(void)
{
}
void board_deinit(void)
{

}


board_init(user_board_init)
