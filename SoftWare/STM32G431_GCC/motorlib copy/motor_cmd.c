#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "motor.h"
// 命令映射表结构
typedef struct
{
    const char *cmd_name;
    uint8_t min_params; // 最少需要的参数个数
    enum foc_parameters_index data_index;
} command_map_t;

// 命令表定义
static const command_map_t cmd_map[] = {
    {"D_PI", 2, INDEX_D_PI},
    {"D_ref", 1, INDEX_ID_REF},

    {"Velocity_PI", 2, INDEX_VELOCITY_PI},
    {"Valocity_tar", 1, INDEX_VELOCITY_REG},

    {"Pos_PI", 2, INDEX_POS_PI},
    {"Pos_tar", 1, INDEX_POSITION_TAR},

    {"VP_PI", 4, INDEX_VP_PI},

};
void process_data(uint8_t *data, uint16_t len)
{

    if (data[0] == 0 || len == 0 || len > 255)
    {
        return;
    }

    char buf[256];
    uint16_t copy_len = (len < sizeof(buf) - 1) ? len : sizeof(buf) - 1;
    memcpy(buf, (char *)data, copy_len);
    buf[copy_len] = '\0';

    // 找冒号
    char *colon_pos = strchr(buf, ':');
    if (!colon_pos)
    {
        return;
    }

    *colon_pos = '\0';
    char *cmd = buf;
    char *params_str = colon_pos + 1;

    // ========== 高效参数解析 ==========
    float params[10];
    uint8_t param_count = 0;
    char *ptr = params_str;

    while (*ptr != '\0' && param_count < 10)
    {
        // 跳过空格
        while (*ptr == ' ')
        {
            ptr++;
        }
        if (*ptr == '\0')
        {
            break;
        }

        // 解析浮点数
        char *end;
        params[param_count] = strtof(ptr, &end);

        if (end == ptr)
        {
            // 转换失败，跳过这个字段
            while (*end != ',' && *end != '\0')
            {
                end++;
            }
        }
        else
        {
            param_count++;
        }

        // 移动到下一个参数
        ptr = end;
        if (*ptr == ',')
        {
            ptr++;
        }
        else
        {
            break;
        }
    }

    if (param_count == 0)
    {
        return;
    }

    for (uint8_t i = 0; i < sizeof(cmd_map) / sizeof(cmd_map[0]); i++)
    {
        if (strcmp(cmd, cmd_map[i].cmd_name) == 0)
        {
            if (param_count >= cmd_map[i].min_params)
            {
                float input[10];
                uint8_t copy_count = (param_count < 10) ? param_count : 10;

                for (int j = 0; j < copy_count; j++)
                {
                    input[j] = params[j];
                }

                debug_update_foc_data(input, cmd_map[i].data_index);
            }
            return;
        }
    }
}