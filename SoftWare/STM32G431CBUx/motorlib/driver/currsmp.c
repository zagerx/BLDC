#include "currsmp.h"
#include "device.h"

/**
 * @brief 在设备初始化时计算电流转换增益
 * @param cfg 设备配置指针
 */
void currsmp_init(struct currsmp_t *currsmp)
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
	cfg->gain_vbus = (((cfg->r2 + cfg->r1) / cfg->r1) * (cfg->vol_ref / cfg->adc_rang));
}
void currsmp_update_currents(struct currsmp_t *currsmp, float *iabc)
{
	struct currsmp_data *data = (struct currsmp_data *)currsmp->data;
	struct currsmp_config *cfg = currsmp->config;

	data->ia = ((float)data->channle_raw_a - (float)cfg->params->offset_a) * cfg->gain;
	data->ib = ((float)data->channle_raw_b - (float)cfg->params->offset_b) * cfg->gain;
	data->ic = ((float)data->channle_raw_c - (float)cfg->params->offset_c) * cfg->gain;
	data->vbus = (float)data->channle_raw_vbus * cfg->gain_vbus;
	iabc[0] = data->ia;
	iabc[1] = data->ib;
	iabc[2] = data->ic;
	iabc[3] = data->vbus;
}
float get_currsmp_vbus(struct currsmp_t *currsmp)
{
	struct currsmp_data *data = (struct currsmp_data *)currsmp->data;
	return data->vbus;
}
void currsmp_update_raw(struct currsmp_t *currsmp, uint32_t *adc_raw)
{
	struct currsmp_data *data = (struct currsmp_data *)currsmp->data;
	data->channle_raw_a = adc_raw[0];
	data->channle_raw_b = adc_raw[1];
	data->channle_raw_c = adc_raw[2];
	data->channle_raw_vbus = adc_raw[3];
}
void _read_currsmp_raw(struct currsmp_t *currsmp, uint32_t *raw_buf)
{
	struct currsmp_data *data = (struct currsmp_data *)currsmp->data;
	raw_buf[0] = data->channle_raw_a;
	raw_buf[1] = data->channle_raw_b;
	raw_buf[2] = data->channle_raw_c;
}
void _update_currsmp_offset(struct currsmp_t *currsmp, uint32_t *raw_buf)
{
	struct currsmp_config *cfg = currsmp->config;
	cfg->params->offset_a = raw_buf[0];
	cfg->params->offset_b = raw_buf[1];
	cfg->params->offset_c = raw_buf[2];
}
