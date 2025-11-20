#ifndef CURRSMP_H
#define CURRSMP_H

#include "stdint.h"
#include "device.h"
struct currsmp_data {
	uint32_t channle_raw_a;
	uint32_t channle_raw_b;
	uint32_t channle_raw_c;

	uint32_t offset_a;
	uint32_t offset_b;
	uint32_t offset_c;

	float factor;

	float ia;
	float ib;
	float ic;
};

// struct currsmp_api
// {
// };

void currsmp_updata(struct device *dev);
void currsmp_caribe_offset(struct device *dev);
void currsmp_update_raw(struct device *currsmp, uint32_t *adc_raw);

#endif
