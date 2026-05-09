#include "gpio.h"
#include <avr/io.h>
#include "bit_math.h"

void GPIO_SetPinDirection(uint8 port, uint8 pin, uint8 direction)
{
    switch(port)
    {
        case PORT_A:
            if(direction == GPIO_OUTPUT) SET_BIT(DDRA, pin);
            else CLEAR_BIT(DDRA, pin);
            break;

        case PORT_B:
            if(direction == GPIO_OUTPUT) SET_BIT(DDRB, pin);
            else CLEAR_BIT(DDRB, pin);
            break;

        case PORT_C:
            if(direction == GPIO_OUTPUT) SET_BIT(DDRC, pin);
            else CLEAR_BIT(DDRC, pin);
            break;

        case PORT_D:
            if(direction == GPIO_OUTPUT) SET_BIT(DDRD, pin);
            else CLEAR_BIT(DDRD, pin);
            break;

        case PORT_E:
            if(direction == GPIO_OUTPUT) SET_BIT(DDRE, pin);
            else CLEAR_BIT(DDRE, pin);
            break;

        case PORT_F:
            if(direction == GPIO_OUTPUT) SET_BIT(DDRF, pin);
            else CLEAR_BIT(DDRF, pin);
            break;

        case PORT_G:
            if(direction == GPIO_OUTPUT) SET_BIT(DDRG, pin);
            else CLEAR_BIT(DDRG, pin);
            break;

        case PORT_H:
            if (direction == GPIO_OUTPUT) SET_BIT(DDRH, pin);
            else CLEAR_BIT(DDRH, pin);
            break;

        default:
            break;
    }
}

void GPIO_WritePin(uint8 port, uint8 pin, uint8 value)
{
    switch(port)
    {
        case PORT_A:
            if(value == GPIO_HIGH) SET_BIT(PORTA, pin);
            else CLEAR_BIT(PORTA, pin);
            break;

        case PORT_B:
            if(value == GPIO_HIGH) SET_BIT(PORTB, pin);
            else CLEAR_BIT(PORTB, pin);
            break;

        case PORT_C:
            if(value == GPIO_HIGH) SET_BIT(PORTC, pin);
            else CLEAR_BIT(PORTC, pin);
            break;

        case PORT_D:
            if(value == GPIO_HIGH) SET_BIT(PORTD, pin);
            else CLEAR_BIT(PORTD, pin);
            break;

        case PORT_E:
            if(value == GPIO_HIGH) SET_BIT(PORTE, pin);
            else CLEAR_BIT(PORTE, pin);
            break;

        case PORT_F:
            if(value == GPIO_HIGH) SET_BIT(PORTF, pin);
            else CLEAR_BIT(PORTF, pin);
            break;

        case PORT_G:
            if(value == GPIO_HIGH) SET_BIT(PORTG, pin);
            else CLEAR_BIT(PORTG, pin);
            break;

        case PORT_H:
            if (value == GPIO_HIGH) SET_BIT(PORTH, pin);
            else CLEAR_BIT(PORTH, pin);
            break;

        default:
            break;        
    }
}

uint8 GPIO_ReadPin(uint8 port, uint8 pin)
{
    switch(port)
    {
        case PORT_A: return GET_BIT(PINA, pin);
        case PORT_B: return GET_BIT(PINB, pin);
        case PORT_C: return GET_BIT(PINC, pin);
        case PORT_D: return GET_BIT(PIND, pin);
        case PORT_E: return GET_BIT(PINE, pin);
        case PORT_F: return GET_BIT(PINF, pin);
        case PORT_G: return GET_BIT(PING, pin);
        case PORT_H: return GET_BIT(PINH, pin);
    }
    return 0;
}