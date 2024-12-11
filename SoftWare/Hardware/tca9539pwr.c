#include "tac9539pwr.h"
#include "debuglog.h"
#include "i2c.h"
#include "main.h"
#define TCA9539_I2C_ADDRESS       (0x74)
#define TCA9539_I2C_ADDRESS_WRITE ((TCA9539_I2C_ADDRESS<<1) | 0x00)
#define TCA9539_I2C_ADDRESS_READ  ((TCA9539_I2C_ADDRESS<<1) | 0x01)

// GPIO
#define GPIO_PORT_RESET (GPIOE)
#define GPIO_PIN_RESET  (GPIO_PIN_15)

// define for TCA9539 register, 以下是TCA9539的寄存器地址定义
#define INPUT_PORT_REGISTER_0     (0x00)
#define INPUT_PORT_REGISTER_1     (0x01)
#define OUTPUT_PORT_REGISTER_0    (0x02)
#define OUTPUT_PORT_REGISTER_1    (0x03)
#define POLARITY_INVERSION_REGISTER_0 (0x04)
#define POLARITY_INVERSION_REGISTER_1 (0x05)

#define CONFIGURATION_REGISTER_0  (0x06)
#define CONFIGURATION_REGISTER_1  (0x07)
// define for input port pos
#define INPUT_PORT_POS_A1   (0)
#define INPUT_PORT_POS_A2   (1)
#define INPUT_PORT_POS_A3   (2)
#define INPUT_PORT_POS_A4   (3)
#define INPUT_PORT_POS_ID0  (4)
#define INPUT_PORT_POS_ID1  (5)

// define for output port pos
#define OUTPUT_PORT_POS_WP_EEPROM (0)
#define OUTPUT_PORT_POS_RUN       (1)
#define OUTPUT_PORT_POS_VCC3V3_EN (2)
#define OUTPUT_PORT_POS_POWER_OFF (3)

void tca9539pwr_init(i2c_bus_t *bus)
{
  // HAL_GPIO_WritePin(GPIO_PORT_RESET, GPIO_PIN_RESET, 1); 
  //config port0 as output
  uint8_t u1Buf[2] = {0};
  u1Buf[0] = 0x00;
  bus->write(TCA9539_I2C_ADDRESS_WRITE,CONFIGURATION_REGISTER_1,u1Buf,1);
  // write value
  for (unsigned char i = 0; i < 20; i++)
  {
    USER_DEBUG_NORMAL("SSSS\n");
    u1Buf[0] = 1<<1;
    bus->write(TCA9539_I2C_ADDRESS_WRITE,CONFIGURATION_REGISTER_1,u1Buf,1);    
    HAL_Delay(500);
    u1Buf[0] = 0<<1;
    bus->write(TCA9539_I2C_ADDRESS_WRITE,CONFIGURATION_REGISTER_1,u1Buf,1);    
    HAL_Delay(500);
  }
}

void tca9539pwr_read(i2c_bus_t *bus)
{
  
}


