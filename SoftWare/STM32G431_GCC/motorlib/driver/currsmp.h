#ifndef CURRSMP_H
#define CURRSMP_H

#include "stdint.h"
#include "device.h"
struct currsmp_config
{
	float rs;
	float opm;
	float vol_ref;
	float adc_rang;
	float gain;
	float r1;
	float r2;
	float gain_vbus;
};
struct currsmp_data
{
	uint32_t channle_raw_a;
	uint32_t channle_raw_b;
	uint32_t channle_raw_c;
	uint32_t channle_raw_vbus;
	uint32_t offset_a;
	uint32_t offset_b;
	uint32_t offset_c;

	float ia;
	float ib;
	float ic;
	float vbus;
};
void currsmp_init(struct device *currsmp);
void currsmp_update_currents(struct device *dev, float *iabc);
void currsmp_caribe_offset(struct device *dev);
void currsmp_update_raw(struct device *currsmp, uint32_t *adc_raw);
float get_currsmp_vbus(struct device *dev);

#endif
