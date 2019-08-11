#ifndef __ADXL345_H
#define __ADXL345_H

#include "main.h"
#include "i2c.h"

#define adxl_address 0x53<<1

void adxl_init (void);
void adxl_write (uint8_t reg, uint8_t value);
void adxl_read_values (uint8_t reg,uint8_t *data_xyz);
void adxl_read_address (uint8_t reg);
void get_data_array(float *data_xyz);


#endif

