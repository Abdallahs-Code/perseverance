#include "gpio.h"
#include <avr/io.h>
#include "bit_math.h"

void GPIO_SetPinDirection(uint8 port, uint8 pin, uint8 direction)
{
    switch(port)
    {
        case PORT_B:
            if(direction == GPIO_OUTPUT)
                SET_BIT(DDRB, pin);
            else
                CLEAR_BIT(DDRB, pin);
            break;

        case PORT_C:
            if(direction == GPIO_OUTPUT)
                SET_BIT(DDRC, pin);
            else
                CLEAR_BIT(DDRC, pin);
            break;

        case PORT_D:
            if(direction == GPIO_OUTPUT)
                SET_BIT(DDRD, pin);
            else
                CLEAR_BIT(DDRD, pin);
            break;
    }
}

void GPIO_WritePin(uint8 port, uint8 pin, uint8 value)
{
    switch(port)
    {
        case PORT_B:
            if(value == GPIO_HIGH)
                SET_BIT(PORTB, pin);
            else
                CLEAR_BIT(PORTB, pin);
            break;

        case PORT_C:
            if(value == GPIO_HIGH)
                SET_BIT(PORTC, pin);
            else
                CLEAR_BIT(PORTC, pin);
            break;

        case PORT_D:
            if(value == GPIO_HIGH)
                SET_BIT(PORTD, pin);
            else
                CLEAR_BIT(PORTD, pin);
            break;
    }
}

uint8 GPIO_ReadPin(uint8 port, uint8 pin)
{
    switch(port)
    {
        case PORT_B: return GET_BIT(PINB, pin);
        case PORT_C: return GET_BIT(PINC, pin);
        case PORT_D: return GET_BIT(PIND, pin);
    }
    return 0;
}