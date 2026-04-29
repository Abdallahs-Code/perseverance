#ifndef GPIO_H
#define GPIO_H

#include "std_types.h"

// Ports
#define PORT_B 0
#define PORT_C 1
#define PORT_D 2

// Direction
#define GPIO_OUTPUT 1
#define GPIO_INPUT  0

// Value
#define GPIO_HIGH 1
#define GPIO_LOW  0

#ifdef __cplusplus
extern "C" {
#endif

void GPIO_SetPinDirection(uint8 port, uint8 pin, uint8 direction);
void GPIO_WritePin(uint8 port, uint8 pin, uint8 value);
uint8 GPIO_ReadPin(uint8 port, uint8 pin);

#ifdef __cplusplus
}
#endif

#endif