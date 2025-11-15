#include "stm32_as5047.h"
// #include "spi.h"
#include "stdint.h"
/*
 *  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
 *  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
 *  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
 */

static uint8_t ams_parity(uint16_t v) {
  v ^= v >> 8;
  v ^= v >> 4;
  v ^= v >> 2;
  v ^= v >> 1;
  return v & 1;
}

uint16_t as5047_read_raw(void) {

  uint16_t pos, rawVal;
  //   HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_RESET);
  //   HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)pw, (uint8_t *)pr, datalen,
  //   1000); HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin,
  //   GPIO_PIN_SET);

  if (ams_parity(rawVal)) {
    return 0xFFFF;
  } else {
    return pos = (rawVal & 0x3fff);
  }
}
