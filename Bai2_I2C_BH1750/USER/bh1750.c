#include "bh1750.h"
#include "app_i2c.h"  
#include "tim2.h"     


void BH1750_Init(void)
{
    I2C1_Config();
    I2C1_Write(BH1750_ADDR, 0, BH1750_POWER_ON);
    I2C1_Write(BH1750_ADDR, 0, BH1750_ONE_H_MODE);
    Delay_ms(180);
}

float BH1750_ReadLux(void)
{
    uint8_t tmp[2];
    uint16_t raw_val;
    float lux;

    I2C1_Read_Buffer(BH1750_ADDR, 0, tmp, 2);
    raw_val = (tmp[0] << 8) | tmp[1];
    lux = (float)raw_val / 1.2f;

    return lux;
}
