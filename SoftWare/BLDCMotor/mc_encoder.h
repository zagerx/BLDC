#ifndef __ANGLE_ENCODER__H
#define __ANGLE_ENCODER__H
#include "motorctrl_common.h"
#include "stdint.h"
#include "stdbool.h"
void mc_encoder_init(mc_encoder_t *encoder);
void mc_encoder_read(mc_encoder_t *encoder);
void mc_encoder_update(mc_encoder_t *encoder);
void mc_encoder_calibrate(mc_encoder_t *encoder);
void mc_encoder_deinit(mc_encoder_t *encoder);


#endif
