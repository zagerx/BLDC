#include "analog_i2c.h"
#include "analog_i2c_cfg.h"
/*----------------------------------------------------------------------

改文件为i2c总线的操作方法

------------------------------------------------------------------------*/
static void i2c_bitops_start(i2c_bus_t *i2c_bus)
{	
//	i2c_bus->set_sda_out();
	i2c_bus->set_sda(i2c_bus->sda, 1);
	i2c_bus->set_scl(i2c_bus->scl, 1);
	i2c_bus->delayus(10);
	i2c_bus->set_sda(i2c_bus->sda,0);
	i2c_bus->delayus(10);
	i2c_bus->set_scl(i2c_bus->scl,0);
}
static void i2c_bitops_stop(i2c_bus_t *i2c_bus)
{
//	i2c_bus->set_sda_out();
	i2c_bus->set_sda(i2c_bus->sda,0);				//stop:when CLK is high DATA change form low to high
	i2c_bus->set_scl(i2c_bus->scl,1);
	i2c_bus->delayus(3);
	i2c_bus->set_sda(i2c_bus->sda,1);								
	i2c_bus->delayus(5);	
}
static void i2c_bitops_create_ack(i2c_bus_t *i2c_bus)
{
	i2c_bus->set_scl(i2c_bus->scl,0);
	i2c_bus->set_sda_out(i2c_bus->sda);
	i2c_bus->set_sda(i2c_bus->sda,0);
	i2c_bus->delayus(5);
	i2c_bus->set_scl(i2c_bus->scl,1);
	i2c_bus->delayus(5);
	i2c_bus->set_scl(i2c_bus->scl,0);	
}
static void i2c_bitops_create_nack(i2c_bus_t *i2c_bus)
{
	i2c_bus->set_scl(i2c_bus->scl,1);	
	i2c_bus->set_sda_out(i2c_bus->sda);
	i2c_bus->set_sda(i2c_bus->sda,1);
	i2c_bus->delayus(2);
	i2c_bus->set_scl(i2c_bus->scl,1);
	i2c_bus->delayus(2);
	i2c_bus->set_scl(i2c_bus->scl,0);	
}
static char i2c_bitops_wait_ack(i2c_bus_t *i2c_bus)
{
   uint8_t ucErrTime = 0;
   i2c_bus->set_sda(i2c_bus->sda,1);
   i2c_bus->delayus(3);;
	 i2c_bus->set_sda_in(i2c_bus->sda);
   i2c_bus->set_scl(i2c_bus->scl,1);
   i2c_bus->delayus(5);;

    while ( i2c_bus->get_sda(i2c_bus->sda) ) {
        ucErrTime++;
        if ( ucErrTime > 250 ) {
            return 1;
        }
    }
    i2c_bus->set_scl(i2c_bus->scl,0);
    return 0;	
}

static void i2c_bitops_send_byte(i2c_bus_t *i2c_bus,unsigned char data)
{
    uint8_t t;
    i2c_bus->set_sda_out(i2c_bus->sda);
    i2c_bus->set_scl(i2c_bus->scl,0);
    for ( t = 0; t < 8; t++ ){
			if ((data & 0x80 ) >> 7 ){
					i2c_bus->set_sda(i2c_bus->sda,1);
			} else {
					i2c_bus->set_sda(i2c_bus->sda,0);
			}
			data <<= 1;
			i2c_bus->delayus(3);
			i2c_bus->set_scl(i2c_bus->scl,1);
			i2c_bus->delayus(3);
			i2c_bus->set_scl(i2c_bus->scl,0);
			i2c_bus->delayus(3);
    }
}
static unsigned char i2c_bitops_recv_byte(i2c_bus_t *i2c_bus)
{	
    unsigned char i, receive = 0;
		i2c_bus->set_sda_in(i2c_bus->sda);
	
    for ( i = 0; i < 8; i++ ){
			i2c_bus->set_scl(i2c_bus->scl,0);
			i2c_bus->delayus(5);
			i2c_bus->set_scl(i2c_bus->scl,1);
			receive <<= 1;
			if(i2c_bus->get_sda(i2c_bus->sda)) 
			{
				receive++;
			}
			i2c_bus->delayus(5);
    }
		return receive;
}

static unsigned long i2c_bitops_write(i2c_bus_t *i2c_bus,i2c_dev_info_t *msgs)
{
	unsigned long count = 0;
	unsigned char *ptr = NULL;
	count = msgs->size;
	ptr   = msgs->buff;
	while(count > 0)
	{
		i2c_bitops_send_byte(i2c_bus,*ptr);
		ptr++;
		count--;
		if (count != 0)
			i2c_bitops_create_ack(i2c_bus); //发送ACK
		else if(count ==0)
			i2c_bitops_create_nack(i2c_bus);	//
	}
	return 0;
}


static unsigned long i2c_bitops_read(i2c_bus_t *i2c_bus,i2c_dev_info_t *msgs)
{
	unsigned long count = 0,bytes = 0;
	unsigned char *ptr = NULL;
	unsigned char data = 0;
	count = msgs->size;
	ptr   = msgs->buff;
	while(count > 0)
	{
		data = i2c_bitops_recv_byte(i2c_bus);
		*ptr = data;
		bytes++;		
		ptr++;
		count--;
		if (count != 0)
			i2c_bitops_create_ack(i2c_bus); //发送ACK
		else if(count ==0)
			i2c_bitops_create_nack(i2c_bus);	//读完必须发送nACK
	}
	return bytes;	//成功返回读字节数
}
/*	
**接口函数
**...................器件读数据顺序...................................
**	start    +      addr|W/R     waitack     readBytes      stop
**
**...................器件写数据顺序...................................
**	start    +      addr|W/R     waitack     writeBytes      stop
**
**
*/
int i2c_bitops_bus_xfer_read(i2c_bus_t *i2c_bus,i2c_dev_info_t *info)
{
	unsigned char addr = 0;
	addr = info->addr<<1 | 1;
	i2c_bitops_start(i2c_bus);
	i2c_bitops_send_byte(i2c_bus,addr);
	if(!i2c_bitops_wait_ack(i2c_bus))
	{
		i2c_bitops_read(i2c_bus,info);
	}else{
		printf("i2c addr err\r\n");
		return -1;
	}
	i2c_bitops_stop(i2c_bus);
	return 0;
}
int i2c_bitops_bus_xfer_write(i2c_bus_t *i2c_bus,i2c_dev_info_t *info)
{
	unsigned char addr = 0;
	addr = info->addr<<1;
	i2c_bitops_start(i2c_bus);
	i2c_bitops_send_byte(i2c_bus,addr);	
	if(!i2c_bitops_wait_ack(i2c_bus))
	{
		i2c_bitops_write(i2c_bus,info);
	}else{
		printf("i2c addr err\r\n");
		return -1;
	}
	i2c_bitops_stop(i2c_bus);
	return 0;
}

