#include "define.h"       
#include "tim2.h"         
#include "uart.h"         
#include "bh1750.h"       
#include <stdio.h>        

int main(void)
{
    TIM2_Init();
    UART_Init(9600);
    BH1750_Init();
    float lux_value = 0;
    char buffer[50];
    while(1)
    {
        lux_value = BH1750_ReadLux();
        sprintf(buffer, "Lux = %.2f\r\n", lux_value);
        UART_SendString(buffer);
        Delay_ms(1000);
    }
}
