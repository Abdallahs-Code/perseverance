#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

#define LED_PORT   PORT_D
#define LED_PIN    5


#define IN1_PORT PORT_A
#define IN1_PIN  0

#define IN2_PORT PORT_A
#define IN2_PIN  1

#define IN3_PORT PORT_A
#define IN3_PIN  2

#define IN4_PORT PORT_A
#define IN4_PIN  3

#define LEFT_PWM_CHANNEL   TIMER1_CHANNEL_A   // D11
#define RIGHT_PWM_CHANNEL  TIMER1_CHANNEL_B   // D12

#define TRIG_PORT  PORT_B
#define TRIG_PIN   1

#define ECHO_PORT  PORT_B
#define ECHO_PIN   2

#endif