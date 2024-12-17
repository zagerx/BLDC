#include "mc_encoder.h"
#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include "mc_utils.h"
#include "board.h"//TODO
#include "debuglog.h"

//TODO
void mc_encoder_read(mc_encoder_t *encoder)
{
#if (ENCODER_TYPE == ENCODER_TYPE_HALL || ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ || ENCODER_TYPE == ENCODER_TYPE_ABS)
	#ifdef MOTOR_OPENLOOP
		encoder->sensor->self_angle = encoder->self_theta;
		// encoder->set_calib_points((encoder->sensor));
	#endif
		if (encoder->runflag)
		{
			#ifdef MCB_V06
				encoder->update((encoder->sensor));//MCB_V06
			#endif
			encoder->cacle((encoder->sensor));
			encoder->ele_theta = (encoder->sensor->angle);
			encoder->speed = encoder->sensor->speed;
		}
#endif
}
void mc_encoder_init(mc_encoder_t *encoder)
{
	if (!(encoder->init))
	{
		USER_DEBUG_NORMAL("encoder->init is null\n");
	}
	encoder->init((encoder->sensor));
}





float mc_read_virvalencoder(float ialpha,float ibeta)
{
	return 0.0f;
}
