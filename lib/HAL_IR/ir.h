#ifndef IR_H
#define IR_H

#include "../CORE/std_types.h"
#include "../MCAL_GPIO/gpio.h"
#include "../../include/pin_config.h"

#define IR_DETECTED      1
#define IR_NOT_DETECTED  0


void IR_Init(void);

uint8 IR_LeftRead(void);
uint8 IR_RightRead(void);
uint8 IR_FrontRead(void);


#endif