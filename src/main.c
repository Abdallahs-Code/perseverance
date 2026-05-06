#include "gpio.h"
#include "pin_config.h"
#include "bluetooth.h"

int main(void)
{
    Bluetooth_Init(9600);

    while(1)
    {
        Bluetooth_Send("Hello From UART");

        for(volatile long i = 0; i < 50000; i++);
    }
}