#include "motor.h"
#include "gpio.h"
#include "timer.h"
#include "pin_config.h"

/*
Assumptions:
- Left motor: IN1, IN2
- Right motor: IN3, IN4
- PWM: OC1A = Left speed, OC1B = Right speed
*/

void Motor_Init(void)
{
    // Direction pins
    GPIO_SetPinDirection(IN1_PORT, IN1_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(IN2_PORT, IN2_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(IN3_PORT, IN3_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(IN4_PORT, IN4_PIN, GPIO_OUTPUT);

    // PWM init
    Timer1_PWM_Init();
}

/* LEFT MOTOR */
void Motor_LeftForward(void)
{
    GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_HIGH);
    GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_LOW);
}

void Motor_LeftBackward(void)
{
    GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_LOW);
    GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_HIGH);
}

void Motor_LeftStop(void)
{
    GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_LOW);
    GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_LOW);
}

/* RIGHT MOTOR */
void Motor_RightForward(void)
{
    GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_HIGH);
    GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_LOW);
}

void Motor_RightBackward(void)
{
    GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_LOW);
    GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_HIGH);
}

void Motor_RightStop(void)
{
    GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_LOW);
    GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_LOW);
}

/* SPEED CONTROL (PWM) */
void Motor_SetLeftSpeed(uint8 duty)
{
    Timer1_SetDuty(TIMER1_CHANNEL_A, duty);
}

void Motor_SetRightSpeed(uint8 duty)
{
    Timer1_SetDuty(TIMER1_CHANNEL_B, duty);
}