#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"
#include "app_i2c.h"
#include "bh1750.h"

void I2C1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef  I2C_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    I2C_DeInit(I2C1);
    
    I2C_InitStructure.I2C_ClockSpeed          = 200000;
    I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1         = 0x00;
    I2C_InitStructure.I2C_Ack                 = I2C_Ack_Disable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStructure);
    
    I2C_Cmd(I2C1, ENABLE);
}

void I2C1_Write(uint8_t HW_address, uint8_t sub, uint8_t data)
{
    uint32_t timeout = 0;
    
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(++timeout > I2C_TIMEOUT) return;
    }
    
    I2C_Send7bitAddress(I2C1, HW_address, I2C_Direction_Transmitter);
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if(++timeout > I2C_TIMEOUT) return;
    }
    
    I2C_SendData(I2C1, sub);
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if(++timeout > I2C_TIMEOUT) return;
    }
    
    I2C_SendData(I2C1, data);
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if(++timeout > I2C_TIMEOUT) return;
    }
    
    I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t I2C1_Read(uint8_t HW_address, uint8_t sub)
{
    uint8_t data = 0;
    uint32_t timeout = 0;
    
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(++timeout > I2C_TIMEOUT) return 0;
    }
    
    I2C_Send7bitAddress(I2C1, HW_address, I2C_Direction_Transmitter);
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if(++timeout > I2C_TIMEOUT) return 0;
    }
    
    I2C_SendData(I2C1, sub);
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if(++timeout > I2C_TIMEOUT) return 0;
    }
    
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(++timeout > I2C_TIMEOUT) return 0;
    }
    
    I2C_Send7bitAddress(I2C1, HW_address, I2C_Direction_Receiver);
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if(++timeout > I2C_TIMEOUT) return 0;
    }
    
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
        if(++timeout > I2C_TIMEOUT) return 0;
    }
    data = I2C_ReceiveData(I2C1);
    
    I2C_GenerateSTOP(I2C1, ENABLE);
    
    return data;
}

void I2C1_Read_Buffer(uint8_t HW_address, uint8_t sub, uint8_t *p_buf, uint8_t buf_size)
{
    uint32_t timeout = 0;
    uint8_t i = 0;
    
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(++timeout > I2C_TIMEOUT) return;
    }
    
    I2C_Send7bitAddress(I2C1, HW_address, I2C_Direction_Transmitter);
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if(++timeout > I2C_TIMEOUT) return;
    }
    
    I2C_SendData(I2C1, sub);
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if(++timeout > I2C_TIMEOUT) return;
    }
    
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if(++timeout > I2C_TIMEOUT) return;
    }
    
    I2C_Send7bitAddress(I2C1, HW_address, I2C_Direction_Receiver);
    timeout = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if(++timeout > I2C_TIMEOUT) return;
    }
    
    for(i = 0; i < buf_size; i++)
    {
        timeout = 0;
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            if(++timeout > I2C_TIMEOUT) return;
        }
        p_buf[i] = I2C_ReceiveData(I2C1);
    }
    
    I2C_GenerateSTOP(I2C1, ENABLE);
}