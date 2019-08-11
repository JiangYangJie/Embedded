#ifndef __GPIO_IIC_H
#define __GPIO_IIC_H

#include "main.h"

#define IIC_WRITE 0x00;

void iic_start(void);

void iic_stop(void);

void iic_sendbyte(uint8_t byte);

uint8_t iic_readbyte(unsigned char ack);

uint8_t iic_wait_ack(void);

void iic_ack(void);

void iic_nack(void);

uint8_t iic_check_device_status(uint8_t device_addr);















#endif

