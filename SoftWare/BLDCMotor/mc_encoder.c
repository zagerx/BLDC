#include "mc_encoder.h"
#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include "mc_utils.h"
#include "board.h"//TODO
#include "debuglog.h"
#include "mc_focmath.h"
/*==========================================================================================
 * @brief        读取编码器值
 * @FuncName     
 * @param        encoder 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void mc_encoder_read(mc_encoder_t *encoder)
{
#if (ENCODER_TYPE == ENCODER_TYPE_HALL || ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ || ENCODER_TYPE == ENCODER_TYPE_ABS)
	#if(MOTOR_WORK_MODE == MOTOR_DEBUG_SELF_MODE)
		encoder->sensor->self_angle = encoder->self_theta;
	#endif
		if (encoder->runflag)
		{
			#if defined(MCB_V06) || defined(STM32G4CUBX)
				mc_encoder_update(encoder);
			#endif
			//计算角度、速度
			encoder->sensor->cacle(encoder->sensor);
			//更新当前角度、速度
			// encoder->ele_theta = _normalize_angle((encoder->sensor->angle + MEC_ANGLE_OFFSET)*MOTOR_PAIRS);
			encoder->ele_theta = _normalize_angle((encoder->sensor->angle + MEC_ANGLE_OFFSET));
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
		return;
	}
	encoder->runflag = 0;
	encoder->ele_theta = 0.0f;
	encoder->self_te = 0.0f;
	encoder->speed = 0.0f;
	encoder->total_realmectheta = 0.0f;
	encoder->sensor->init((encoder->sensor));
}
/*==========================================================================================
 * @brief        编码器初始化
 * @FuncName     
 * @param        encoder 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void mc_encoder_deinit(mc_encoder_t *encoder)
{
	encoder->runflag = 0;
	encoder->ele_theta = 0.0f;
	encoder->self_te = 0.0f;
	encoder->speed = 0.0f;
	encoder->total_realmectheta = 0.0f;
	encoder->sensor->deinit((encoder->sensor));
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

void mc_encoder_calibrate(mc_encoder_t *encoder)
{
	encoder->sensor->get_first_points(encoder->sensor);
	#if (ENCODER_TYPE == ENCODER_TYPE_HALL || ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ)
		encoder->ele_theta =  encoder->sensor->angle;
	#endif
	encoder->sensor->set_calib_points(encoder->sensor);
	encoder->runflag = 1;
}

float mc_read_virvalencoder(float ialpha,float ibeta)
{
	return 0.0f;
}
