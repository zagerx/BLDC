#ifndef FEEDBACK_H
#define FEEDBACK_H
#include "device.h"
#include "stdint.h"
struct feedback_config
{
    uint16_t (*get_raw)(void);
};

struct feedback_data
{
    uint16_t raw;
    float e_angle;
};

struct feedback_api
{
    float (*calce_angle)(struct device*);
};

void feedback_update_angle_vec(struct device *dev);


#endif
