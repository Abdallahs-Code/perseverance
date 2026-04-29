#include "ultrasonic.h"
#include <util/delay.h>
#include <avr/io.h>

static void sendTrigger(void) {
    // TRIG LOW
    GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_LOW);
    _delay_us(2);

    // TRIG HIGH 
    GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_HIGH);
    _delay_us(TRIGGER_PULSE_US);

    // TRIG LOW
    GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_LOW);
}

// Return the duration in which echo pin stays high in microseconds
static unsigned long measureEchoPulse(void) {
    unsigned long start;
    unsigned long begin = 0;

    // Wait for ECHO to go HIGH
    while (GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_LOW) {
        if (begin >= MAX_ECHO_US) return 0;  // timeout
        begin++;
    }

    // ECHO is now HIGH, start timing
    start = 0;

    // Wait for ECHO to go LOW
    while (GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_HIGH) {
        if (start >= MAX_ECHO_US) return 0;  // timeout
        start++;
    }

    // Return how long ECHO stayed HIGH
    return start;
}

void ultrasonicBegin(void) {
    // Set TRIG pin as OUTPUT
    GPIO_SetPinDirection(TRIG_PORT, TRIG_PIN, GPIO_OUTPUT);

    // Set ECHO pin as INPUT
    GPIO_SetPinDirection(ECHO_PORT, ECHO_PIN, GPIO_INPUT);

    // Make sure TRIG starts LOW
    GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_LOW);

    _delay_ms(MIN_STARTUP_DELAY_MS);
}

unsigned long ultrasonicReadRawPulse(void) {
    sendTrigger();
    return measureEchoPulse();
}

float ultrasonicReadDistanceCm(void) {
    unsigned long rawTime = ultrasonicReadRawPulse();
    if (rawTime == 0) return -1.0f;
    return (rawTime * SOUND_SPEED_CM_US) / 2.0f;
}