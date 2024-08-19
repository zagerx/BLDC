#include "analog_i2c.h"
#include "analog_i2c_cfg.h"
#include "analog_i2c_com.h"
/*-----------------------------------------------------------------------
该文件主要初始化
I2C总线相关信息

需要外部配置文件来配置到底有几条I2C总线
总线的IO具体

对外提供的信息:
i2c_bus_t* i2c_bus_register(char* bus_name,char *io_scl_name,char* io_sda_name)
------------------------------------------------------------------------*/
/*---------------------------------总线IO口的具体实现---------------------------------*/
static void gpio_set_sda(gpio_pin_t *gpio,int8_t state)
{
	gpio_set_pinstatus(gpio,state);
}
static void gpio_set_scl(gpio_pin_t *gpio,int8_t state)
{
	gpio_set_pinstatus(gpio,state);
}

static unsigned char gpio_get_sda(gpio_pin_t *gpio)
{
	return (gpio_get_pinstatus(gpio));
}

static void gpio_set_sda_out(gpio_pin_t *gpio)
{
	gpio_set_pinmode(gpio,"B.09",PIN_MODE_OUTPUT);
}
static void gpio_set_in(gpio_pin_t *gpio)
{
 	gpio_set_pinmode(gpio,"B.09",PIN_MODE_INPUT);
}

static void gpio_delayus(unsigned int Time)
{
#if 1  /* 不用系统延时时开启这个！！*/
	int i = 9;
	Time *= i;
	while (Time--);
#else
		Delayus(us);
#endif
}
/*------------------------------------------------------------------------*/


/*-------------------------I2C总线的初始化------------------------------------*/
static i2c_bus_t* i2c_bus_register(i2c_bus_info_t *info)
{
    i2c_bus_t *i2c_bus;
    i2c_bus = heap_malloc(sizeof(i2c_bus_t));

    gpio_pin_t *gpio_scl;
    gpio_pin_t *gpio_sda;
    gpio_sda = heap_malloc(sizeof(gpio_pin_t));
    gpio_scl = heap_malloc(sizeof(gpio_pin_t));

	gpio_pininit(gpio_scl,info->i2c_scl,PIN_MODE_OUTPUT);
	gpio_pininit(gpio_sda,info->i2c_sda,PIN_MODE_OUTPUT);    
	gpio_set_pinstatus(gpio_scl,1);
	gpio_set_pinstatus(gpio_sda,1);

    i2c_bus->sda = gpio_sda;
    i2c_bus->scl = gpio_scl;
	i2c_bus->set_sda = gpio_set_sda;
	i2c_bus->get_sda = gpio_get_sda;
	i2c_bus->set_scl = gpio_set_scl;
	i2c_bus->set_sda_in = gpio_set_in;
	i2c_bus->set_sda_out = gpio_set_sda_out;
	i2c_bus->delayus = gpio_delayus;
    i2c_bus->name = info->i2c_name;
    return i2c_bus;
}

/*-----------------根据i2c_bus_infoarry初始化i2c_bus_arry数组 系统调用---------------------*/
void i2c_bus_creat(void)
{
    for (unsigned char i = 0; i < dimof(i2c_bus_infoarry); i++)
    {
        i2c_bus_arry[i] = i2c_bus_register(&i2c_bus_infoarry[i]);
    }
}
/*-------------------------------供I2C设备调用--------------------------------------------*/
i2c_bus_t* i2c_bus_init(i2c_bus_info_t* info)
{
    /*总线遍历功能待完善*/
    for (unsigned char i = 0; i < sizeof(i2c_bus_infoarry); i++)
    {
        /* code */
        if (!strcmp(i2c_bus_arry[i]->name,info->i2c_name))//已经存在
        {
            return i2c_bus_arry[i];
        }
    }
    return 0;
}

/*------------------------------------------------------------------------*/






