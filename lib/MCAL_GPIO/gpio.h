#ifndef GPIO_H
#define GPIO_H

#include "std_types.h"

#define PORT_A 0
#define PORT_B 1
#define PORT_C 2
#define PORT_D 3
#define PORT_E 4
#define PORT_F 5
#define PORT_G 6

#define GPIO_OUTPUT 1
#define GPIO_INPUT  0

#define GPIO_HIGH 1
#define GPIO_LOW  0


void GPIO_SetPinDirection(uint8 port, uint8 pin, uint8 direction);
void GPIO_WritePin(uint8 port, uint8 pin, uint8 value);
uint8 GPIO_ReadPin(uint8 port, uint8 pin);



#endif