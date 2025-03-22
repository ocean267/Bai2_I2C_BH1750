#ifndef _APP_I2C_H
#define _APP_I2C_H

#include "define.h"
#include "stm32f10x.h"

void I2C1_Config(void);
void I2C1_Write(uint8_t HW_address, uint8_t sub, uint8_t data);
uint8_t I2C1_Read(uint8_t HW_address, uint8_t sub);
void I2C1_Read_Buffer(uint8_t HW_address, uint8_t sub, uint8_t *p_buf, uint8_t buf_size);

#endif /* _APP_I2C_H */
