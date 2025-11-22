#include "currsmp.h"
#include "device.h"

/**
 * @brief 在设备初始化时计算电流转换增益
 * @param cfg 设备配置指针
 */
void currsmp_init(struct device *currsmp)
{
	struct currsmp_config *cfg = currsmp->config;
	// 确保分母非零
	if (cfg->rs == 0.0f || cfg->opm == 0.0f || cfg->adc_rang == 0.0f) {
		cfg->gain = 0.0f;
		return;
	}
	// 核心修正：
	// K = V_REF / (N_ADC * R_shunt * A_v)
	cfg->gain = cfg->vol_ref / (cfg->adc_rang * cfg->rs * cfg->opm);
}
void currsmp_update_currents(struct device *dev, float *iabc)
{
	struct currsmp_data *data = (struct currsmp_data *)dev->data;
	struct currsmp_config *cfg = dev->config;

	data->ia = ((float)data->channle_raw_a - (float)data->offset_a) * cfg->gain;
	data->ib = ((float)data->channle_raw_b - (float)data->offset_b) * cfg->gain;
	data->ic = ((float)data->channle_raw_c - (float)data->offset_c) * cfg->gain;
	iabc[0] = data->ia;
	iabc[1] = data->ib;
	iabc[2] = data->ic;
}

void currsmp_update_raw(struct device *currsmp, uint32_t *adc_raw)
{
	struct currsmp_data *data = (struct currsmp_data *)currsmp->data;
	data->channle_raw_a = adc_raw[0];
	data->channle_raw_b = adc_raw[1];
	data->channle_raw_c = adc_raw[2];
}
