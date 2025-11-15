#include "feedback.h"

#include "device.h"

void feedback_update_angle_vec(struct device *dev)
{
    struct feedback_config *cfg=dev->config;
    
    uint16_t raw = cfg->get_raw();

    //calce

    struct feedback_data *data = dev->data;

    data->e_angle = raw/1.0f;
}




