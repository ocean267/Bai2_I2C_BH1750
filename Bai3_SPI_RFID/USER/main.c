#include "tim2.h"
#include "gpio.h"
#include "uart.h"
#include "rfid.h"

int main(void)
{
    Timer2_Init();
    GPIO_Debug_Init();
    USART1_Init(9600);
    Delay_ms(200);
    RFID_Init();
    USART1_Send_String("RFID Reader Initialized\r\n");
    Off();
    while (1)
    {
        RFID_Debug_ReadCard();
        On();
        Delay_ms(1000);
        Off();
        Debug();
    }
}
