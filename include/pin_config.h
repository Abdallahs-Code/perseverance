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
// TRIG pins (any GPIO works — must be header-accessible on Mega 2560 R3)
#define TRIG_FRONT_PIN   4    // PE4 = D2
#define TRIG_LEFT_PIN    5    // PE5 = D3
#define TRIG_RIGHT_PIN   4    // PH4 = D7

// ECHO pins (chosen for ICU/INT support):
//   Front -> PD2 (D19) = INT2,  timestamped against Timer3
//   Left  -> PL0 (D49) = ICP4,  Timer4 input capture
//   Right -> PL1 (D48) = ICP5,  Timer5 input capture

#endif