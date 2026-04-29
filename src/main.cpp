#include "gpio.h"
#include "pin_config.h"

int main(void)
{
    GPIO_SetPinDirection(LED_PORT, LED_PIN, GPIO_OUTPUT);

    while(1)
    {
        GPIO_WritePin(LED_PORT, LED_PIN, GPIO_HIGH);

        for(volatile long i = 0; i < 50000; i++);

        GPIO_WritePin(LED_PORT, LED_PIN, GPIO_LOW);

        for(volatile long i = 0; i < 50000; i++);
    }
}