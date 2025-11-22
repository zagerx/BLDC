#ifndef CURRSMP_H
#define CURRSMP_H

#include "stdint.h"
#include "device.h"
struct currsmp_config {
	float rs;
	float opm;
	float vol_ref;
	float adc_rang;
	float gain;
};
struct currsmp_data {
	uint32_t channle_raw_a;
	uint32_t channle_raw_b;
	uint32_t channle_raw_c;

	uint32_t offset_a;
	uint32_t offset_b;
	uint32_t offset_c;

	float ia;
	float ib;
	float ic;
};
void currsmp_init(struct device *currsmp);
void currsmp_update_currents(struct device *dev, float *iabc);
void currsmp_caribe_offset(struct device *dev);
void currsmp_update_raw(struct device *currsmp, uint32_t *adc_raw);

#endif
