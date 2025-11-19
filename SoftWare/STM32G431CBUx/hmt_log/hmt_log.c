#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "hmt_log.h"

#define COMM_PORT_UART 0
#define COMM_PORT_USB_CDC 1

#define ACTIVE_COMM_PORT COMM_PORT_UART

#if (ACTIVE_COMM_PORT == COMM_PORT_UART)
#include "usart.h"
#elif (ACTIVE_COMM_PORT == COMM_PORT_USB_CDC)
#include "usbd_cdc_if.h"
#include "FreeRTOS.h"
#else
#error "Invalid communication port selection!"
#endif

HAL_StatusTypeDef uart_send_string(const char *str)
{
    if (str == NULL)
    {
        return HAL_ERROR;
    }

    uint16_t len = strlen(str);
    if (len == 0)
    {
        return HAL_OK;
    }
    /* Port-specific transmission */
#if (ACTIVE_COMM_PORT == COMM_PORT_UART)
    return HAL_UART_Transmit(&huart3, (uint8_t *)str, len, 1000);
#elif (ACTIVE_COMM_PORT == COMM_PORT_USB_CDC)
    uint8_t result = CDC_Transmit_FS((uint8_t *)str, len);
    osDelay(1);
    return (result == USBD_OK) ? HAL_OK : HAL_ERROR;
#endif
}

// 提取文件名（去掉路径）
static const char *get_filename(const char *path)
{
    const char *filename = strrchr(path, '/');
    if (filename == NULL)
    {
        filename = strrchr(path, '\\');
    }
    return filename ? filename + 1 : path;
}

void log_output(log_level_t level, const char *file, int line, const char *format, ...)
{
    static const char *level_str[] = {"E", "W", "I", "D"};
    char buffer[256];
    va_list args;
    int prefix_length = 0;

    // 对于 ERROR 和 WARN 级别，添加文件名和行号前缀
    if (level == LOG_ERROR || level == LOG_WARN)
    {
        const char *filename = get_filename(file);
        prefix_length = snprintf(buffer, sizeof(buffer), "[%s][%s:%d] ",
                                 level_str[level], filename, line);
    }

    // 格式化用户消息
    va_start(args, format);
    vsnprintf(buffer + prefix_length, sizeof(buffer) - prefix_length, format, args);
    va_end(args);

    // 确保字符串以换行结束
    size_t current_length = strlen(buffer);
    if (current_length < sizeof(buffer) - 2)
    {
        buffer[current_length++] = '\r';
        buffer[current_length++] = '\n';
        buffer[current_length] = '\0';
    }

    // 输出到串口
    uart_send_string(buffer);
}