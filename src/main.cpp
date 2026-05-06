#include "gpio.h"
#include <util/delay.h>

int main(void)
{
    GPIO_SetPinDirection(PORT_E, 3, GPIO_OUTPUT);

    while (1)
    {
        GPIO_WritePin(PORT_E, 3, GPIO_HIGH);
        for(volatile long i = 0; i < 5000000; i++);

        GPIO_WritePin(PORT_E, 3, GPIO_LOW);
        for(volatile long i = 0; i < 5000000; i++);
    }
}