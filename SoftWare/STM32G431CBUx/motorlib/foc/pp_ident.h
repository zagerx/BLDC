#ifndef PP_IDENT_H
#define PP_IDENT_H

#include <stdbool.h>
#include <stdint.h>

struct pp_ident_config {
  // 施加的开环电角速度(rad/s) 或者 固定步进值(你可任选)
  float openloop_speed;

  // 开环运行时间(秒)，需保证至少旋转机械角度一圈
  float duration;

  // 编码器最大值，例如 AS5047 = 16384
  uint16_t encoder_max;
};

struct pp_ident_data {
  uint16_t raw_start; // 开始时 encoder raw
  uint16_t raw_end;   // 结束时 encoder raw

  int32_t raw_delta; // 机械角度变化（未归一化）
  float mech_rounds; // 机械角度圈数（可能>1或<1）

  float electrical_rounds; // 开环期间走过的电角度圈数（由指令决定）
  uint16_t pole_pairs;     // 识别出的极对数

  bool done; // 是否识别完成
};

struct device;

void pp_ident_start(struct device *dev, struct pp_ident_config *cfg);
void pp_ident_update(struct device *dev, float dt);
bool pp_ident_is_done(struct device *dev);
uint16_t pp_ident_result_pp(struct device *dev);

#endif
