#ifndef __ANALOG_I2C__H
#define __ANALOG_I2C__H
#include "analog_i2c_cfg.h"
#define I2C_BUS_WR            0x0000
#define I2C_BUS_RD			  0x0001
typedef struct i2c_bus_info 
{
    const char *i2c_name;
    const char *i2c_scl;
    const char *i2c_sda;
}i2c_bus_info_t;

typedef struct i2c_dev_mesg
{
    unsigned short  addr;
    unsigned short  flags;
    unsigned short  size;
    unsigned char   *buff;
    unsigned char   retries;
}i2c_dev_info_t;
/*-----------------------I2C总线-------------------------------------------------*/
typedef struct i2c_bus
{
	/*总线操作方法*/
	void (*set_sda)(gpio_pin_t *gpio,int8_t state);
    void (*set_scl)(gpio_pin_t *gpio,int8_t state);
	unsigned char (*get_sda)(gpio_pin_t *gpio);
	void (*delayus)(uint32_t us);
	void (*set_sda_out)(gpio_pin_t *gpio);
	void (*set_sda_in)(gpio_pin_t *gpio);

	/*总线名字*/
	const char *name;//哪个I2C总线
    gpio_pin_t *scl;
    gpio_pin_t *sda;
}i2c_bus_t;
/*------------------------------------------------------------------------------*/

typedef struct i2c_dev{
	i2c_bus_t *i2c_phy;
    i2c_dev_info_t *dev_msg;
}i2c_dev_t;

extern int i2c_bus_xfer_read(i2c_dev_t *i2c_dev);
extern int i2c_bus_xfer_write(i2c_dev_t *i2c_dev);
extern i2c_bus_t* i2c_bus_init(i2c_bus_info_t* info);
#endif
