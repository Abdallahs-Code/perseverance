#include "motor.h"
#include "gpio.h"
#include "timer.h"
#include "pin_config.h"

/*
- Left motor: IN1, IN2
- Right motor: IN3, IN4
- PWM: OC1A = Left motor speed, OC1B = Right motor speed
*/

void Motor_Init(void)
{
    // Set motor pins as output
    GPIO_SetPinDirection(IN1_PORT, IN1_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(IN2_PORT, IN2_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(IN3_PORT, IN3_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(IN4_PORT, IN4_PIN, GPIO_OUTPUT);

    //Starts Timer1 in PWM mode
    // PWM controls motor speed
    Timer1_PWM_Init();
}

// === LEFT MOTOR ===

// Moves left motor forward
// IN1 -> High, IN2 -> Low
void Motor_LeftForward(void)
{
    GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_HIGH);
    GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_LOW);
}

// Moves left motor backward
// IN1 -> Low, IN2 -> High
void Motor_LeftBackward(void)
{
    GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_LOW);
    GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_HIGH);
}

// Stops motor
// IN1 -> Low, IN2 -> Low
void Motor_LeftStop(void)
{
    GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_LOW);
    GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_LOW);
}

// === RIGHT MOTOR ===

// Right motor's behavior is opposite the left motor's behavior
void Motor_RightForward(void)
{
    GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_LOW);
    GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_HIGH);
}

void Motor_RightBackward(void)
{
    GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_HIGH);
    GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_LOW);
}

void Motor_RightStop(void)
{
    GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_LOW);
    GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_LOW);
}

// SPEED CONTROL (PWM)

/* 
Changes PWM duty cycle on channel A
More duty cycle = more speed
Example:
    Duty	Speed
    0	     stop
    50	    medium
    100	     full
*/

void Motor_SetLeftSpeed(uint8 duty)
{
    Timer1_SetDuty(TIMER1_CHANNEL_A, duty + 2);
}

void Motor_SetRightSpeed(uint8 duty)
{
    Timer1_SetDuty(TIMER1_CHANNEL_B, duty);
}

// CAR MOVEMENT CONTROL
void Car_MoveForward(void)
{
    Motor_LeftForward();
    Motor_RightForward();

    Motor_SetLeftSpeed(MOVE_SPEED);
    Motor_SetRightSpeed(MOVE_SPEED);
}

void Car_MoveBackward(void)
{
    Motor_LeftBackward();
    Motor_RightBackward();

    Motor_SetLeftSpeed(MOVE_SPEED);
    Motor_SetRightSpeed(MOVE_SPEED);
}


void Car_TurnRight(void)
{
    Motor_LeftForward();
    Motor_RightBackward();

    Motor_SetLeftSpeed(TURN_SPEED);
    Motor_SetRightSpeed(TURN_SPEED);
}

void Car_TurnLeft(void)
{
    Motor_LeftBackward();
    Motor_RightForward();

    Motor_SetLeftSpeed(TURN_SPEED);
    Motor_SetRightSpeed(TURN_SPEED);
}


void Car_Stop(void)
{
    Motor_LeftStop();
    Motor_RightStop();

    Motor_SetLeftSpeed(0);
    Motor_SetRightSpeed(0);
}

void Car_RightAlign(void)
{
    Motor_LeftForward();
    Motor_RightForward();

    Motor_SetLeftSpeed(ALIGN_LOW_SPEED);
    Motor_SetRightSpeed(ALIGN_HIGH_SPEED);
}

void Car_LeftAlign(void)
{
    Motor_LeftForward();
    Motor_RightForward();

    Motor_SetLeftSpeed(ALIGN_HIGH_SPEED);
    Motor_SetRightSpeed(ALIGN_LOW_SPEED);
}