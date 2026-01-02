#include "foc_data.h"

/* -------------------------------------------------------------------------- */
/*                              Test Input Definition                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief Voltage modulation test input
 *
 * 本结构体定义了一次“电压调制测试步”的全部输入量，
 * 对应一组独立的 dq 电压请求与电角度。
 *
 * 成员说明：
 *  - vbus   : 直流母线电压 (Volt)
 *  - eangle : 电角度 (rad)
 *  - ud     : d 轴电压请求 (Volt，未限幅)
 *  - uq     : q 轴电压请求 (Volt，未限幅)
 */
struct test_vol_modul_input {
	float vbus;
	float eangle;
	float ud;
	float uq;
};

/* 单步测试执行函数（内部使用） */
static void test_voltage_modulation_step(struct test_vol_modul_input *in);

/* -------------------------------------------------------------------------- */
/*                              Test Task: Case 1                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief Voltage modulation test task - Case 1
 *
 * 测试目的：
 *  - 验证在【线性区内】电压请求条件下：
 *      dq → αβ → SVPWM 整条调制链路的
 *      - 连续性
 *      - 几何正确性
 *      - 数值稳定性
 *
 * 测试工况定义：
 *  - vbus  = 24V（固定）
 *  - ud    = 0
 *  - uq    = 常数（小于线性区极限）
 *  - eangle 从 0 → 2π 连续扫描
 *
 * 期望现象：
 *  - αβ 平面中为标准圆轨迹
 *  - duty_a / b / c 在 60° 扇区边界连续
 *
 * 调度说明：
 *  - 本函数无实时性要求
 *  - 100 Hz 或更低频率调用即可
 */
void test_voltage_modul_task1(void)
{
	enum {
		START = 0,
		RUNING,
		OVER,
	};

	static int16_t state = START;
	static struct test_vol_modul_input input;

	switch (state) {
	case START:
		/* -------- 初始化测试参数 -------- */
		input.vbus = 24.0f;
		input.eangle = 0.0f;
		input.ud = 0.0f;
		input.uq = 0.0f;

		state = RUNING;
		break;

	case RUNING:
		/* -------- 构造测试激励 --------
		 *
		 * 固定 dq 电压幅值，仅扫描电角度，
		 * 用于验证调制链路的角度连续性
		 */
		input.vbus = 24.0f;
		input.ud = 0.0f;
		input.uq = 3.0f; // 线性区内幅值
		input.eangle = input.eangle;

		test_voltage_modulation_step(&input);

		/* 电角度递增（细步进，便于观察扇区边界） */
		input.eangle += 0.001f;

		if (input.eangle >= 6.28f) {
			state = OVER;
		}
		break;

	case OVER:
		/* 测试结束，保持空转 */
		break;
	}
}

/* -------------------------------------------------------------------------- */
/*                       Voltage Modulation Test Step                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief Execute one voltage modulation test step
 *
 * 本函数执行一次“完整的电压调制链路”，包括：
 *  1. dq 电压矢量限幅（内切圆）
 *  2. dq → αβ 坐标逆变换
 *  3. αβ → SVPWM 七段式映射
 *  4. 打印关键中间量与最终 duty
 *
 * @param in 测试输入参数（不在函数内修改）
 *
 * 串口输出格式（CSV）：
 *  vbus,
 *  eangle,
 *  ud_req, uq_req,
 *  ualpha, ubeta,
 *  duty_a, duty_b, duty_c
 */
static void test_voltage_modulation_step(struct test_vol_modul_input *in)
{
	float vbus = in->vbus;
	float eangle = in->eangle;
	float ud = in->ud;
	float uq = in->uq;

	/* -------- 1. dq 电压矢量限幅（线性区） -------- */
	foc_limit_voltage_vector(vbus, &ud, &uq);

	/* -------- 2. dq → αβ（含归一化） -------- */
	float ualpha, ubeta;
	foc_modulate_dq(eangle, vbus, ud, uq, &ualpha, &ubeta);

	/* -------- 3. αβ → SVPWM 七段式 -------- */
	float duty[3];
	svpwm_seven_segment(ualpha, ubeta, &duty[0], &duty[1], &duty[2]);

	/* -------- 4. 数据输出 --------
	 *
	 * 注意：
	 *  - ud / uq 打印的是“限幅前请求值”
	 *  - ualpha / ubeta 为归一化后的调制空间量
	 */
	printf("%f,%f,%f,%f,%f,%f,%f,%f,%f", vbus, eangle, in->ud, in->uq, ualpha, ubeta, duty[0],
	       duty[1], duty[2]);
}
