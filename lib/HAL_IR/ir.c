#include "ir.h"

void IR_Init(void)
{
    GPIO_SetPinDirection(IR_LEFT_PORT, IR_LEFT_PIN, GPIO_INPUT);
    GPIO_SetPinDirection(IR_RIGHT_PORT, IR_RIGHT_PIN, GPIO_INPUT);
    GPIO_SetPinDirection(IR_FRONT_PORT, IR_FRONT_PIN, GPIO_INPUT);
}

uint8 IR_LeftRead(void)
{
    return !GPIO_ReadPin(IR_LEFT_PORT, IR_LEFT_PIN);
}

uint8 IR_RightRead(void)
{
    return !GPIO_ReadPin(IR_RIGHT_PORT, IR_RIGHT_PIN);
}

uint8 IR_FrontRead(void)
{
    return !GPIO_ReadPin(IR_FRONT_PORT, IR_FRONT_PIN);
}