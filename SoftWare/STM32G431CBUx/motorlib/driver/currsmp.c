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

void currsmp_update_raw(struct device *currsmp,uint32_t *adc_raw)
{
    struct currsmp_data *data = (struct currsmp_data *)currsmp->data;
    data->channle_raw_a = adc_raw[0];
    data->channle_raw_b = adc_raw[1];
    data->channle_raw_c = adc_raw[2];
}