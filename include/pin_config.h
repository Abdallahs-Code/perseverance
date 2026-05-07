#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

#define LED_PORT PORT_D
#define LED_PIN  5

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

#define IR_LEFT_PORT   PORT_C
#define IR_LEFT_PIN    0   // D37

#define IR_RIGHT_PORT  PORT_C
#define IR_RIGHT_PIN   1   // D36

#define IR_FRONT_PORT  PORT_C
#define IR_FRONT_PIN   2   // D35

// Ultrasonic sensors
#define TRIG_PORT        PORT_E
#define TRIG_FRONT_PIN   4    // PE4
#define TRIG_LEFT_PIN    5    // PE5
#define TRIG_RIGHT_PIN   6    // PE6

#endif