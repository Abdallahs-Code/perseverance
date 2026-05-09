#ifndef MOTOR_H
#define MOTOR_H

#include "std_types.h"

#define MOVE_SPEED   70
#define TURN_SPEED   70

void Motor_Init(void);

// Left motor
void Motor_LeftForward(void);
void Motor_LeftBackward(void);
void Motor_LeftStop(void);

// Right motor
void Motor_RightForward(void);
void Motor_RightBackward(void);
void Motor_RightStop(void);

// Speed control
void Motor_SetLeftSpeed(uint8 duty);
void Motor_SetRightSpeed(uint8 duty);


void Car_MoveForward(void);
void Car_MoveBackward(void);

void Car_TurnRight(void);
void Car_TurnLeft(void);

void Car_Stop(void);

#endif