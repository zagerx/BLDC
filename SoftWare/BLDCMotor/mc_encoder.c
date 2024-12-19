#include "mc_encoder.h"
#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include "mc_utils.h"
#include "board.h"//TODO
#include "debuglog.h"

/*==========================================================================================
 * @brief        读取编码器值
 * @FuncName     
 * @param        encoder 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void mc_encoder_read(mc_encoder_t *encoder)
{
#if (ENCODER_TYPE == ENCODER_TYPE_HALL || ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ || ENCODER_TYPE == ENCODER_TYPE_ABS)
	#ifdef MOTOR_OPENLOOP
		encoder->sensor->self_angle = encoder->self_theta;
	#endif
		if (encoder->runflag)
		{
			#ifdef MCB_V06
				mc_encoder_update(encoder);
			#endif
			//计算角度、速度
			encoder->sensor->cacle(encoder->sensor);
			//更新当前角度、速度
			encoder->ele_theta = (encoder->sensor->angle);
			encoder->speed = encoder->sensor->speed;
		}
#endif
}
/*==========================================================================================
 * @brief        编码器初始化
 * @FuncName     
 * @param        encoder 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void mc_encoder_init(mc_encoder_t *encoder)
{
	if (!(encoder->sensor->init))
	{
		USER_DEBUG_NORMAL("encoder->init is null\n");
	}
	encoder->sensor->init((encoder->sensor));
}
/*==========================================================================================
 * @brief        编码器更新基准
 * @FuncName     
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void mc_encoder_update(mc_encoder_t *encoder)
{
	encoder->sensor->update_base(encoder->sensor);
}



float mc_read_virvalencoder(float ialpha,float ibeta)
{
	return 0.0f;
}
