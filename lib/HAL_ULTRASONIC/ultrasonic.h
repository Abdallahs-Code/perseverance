#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include <avr/io.h>

// ─────────────────────────────────────────────
//  Pin configuration — ATmega328P / Arduino Uno R3
//
//  TRIG → Arduino Pin 9  → AVR: PB1 (PORTB, bit 1)
//  ECHO → Arduino Pin 10 → AVR: PB2 (PORTB, bit 2)
//
//  To change pins in the future:
//    1. Find the AVR port and bit for your chosen Arduino pin
//       using the Uno R3 pinout diagram (e.g. Pin 8 = PB0, Pin 11 = PB3)
//    2. If the new pin is on a different port (PORTC or PORTD),
//       change DDRB/PORTB/PINB to DDRC/PORTC/PINC or DDRD/PORTD/PIND
//    3. Update the bit numbers below accordingly
// ─────────────────────────────────────────────
#define TRIG_BIT   PB1   // change this if you move the TRIG pin
#define ECHO_BIT   PB2   // change this if you move the ECHO pin

#define SOUND_SPEED_CM_US    0.0343f
#define TRIGGER_PULSE_US     10
#define MAX_ECHO_US          30000UL   // timeout ~5 m range limit
#define MIN_STARTUP_DELAY_MS 60

void ultrasonicBegin();
unsigned long ultrasonicReadRawPulse();
float ultrasonicReadDistanceCm();

#endif