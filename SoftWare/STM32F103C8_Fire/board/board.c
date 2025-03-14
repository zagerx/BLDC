/****************************************************************************
Filename: board.c
Functions list:
Notes:
Copyright (c) 2022, Syrius Robotics
All rights reserved.
****************************************************************************/
#include "board.h"
#include "debuglog.h"
#include "usart.h"
#include "string.h"
static uint8_t sg_uartreceive_buff[125];

void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    if(USART1 == huart->Instance)                                   
    {
        if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart);                     
            HAL_UART_DMAStop(huart);
            unsigned short data_length  = sizeof(sg_uartreceive_buff) - __HAL_DMA_GET_COUNTER(huart->hdmarx);
            USER_DEBUG_NORMAL("datalen %d\r\n",data_length);
            // protocol_getdata_tofifo(sg_uartreceive_buff,data_length);
            memset(sg_uartreceive_buff,0,data_length);
            data_length = 0;
            HAL_UART_Receive_DMA(huart, (uint8_t*)sg_uartreceive_buff, sizeof(sg_uartreceive_buff));
        }
    }
}


