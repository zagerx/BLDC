#include "currsmp.h"

void currsmp_caribe_offset(struct device *dev)
{
    struct currsmp_data *data = (struct currsmp_data *)dev->data;
    data->offset_a = 2048;
}

void currsmp_updata(struct device *dev)
{
    struct currsmp_data *data = (struct currsmp_data *)dev->data;

    uint32_t raw_a = (data->channle_raw_a - data->offset_a);

    data->ia = raw_a * data->factor;
}
