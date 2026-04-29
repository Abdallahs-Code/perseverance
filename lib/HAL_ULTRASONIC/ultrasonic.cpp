#include "ultrasonic.h"

static void sendTrigger() {
    // TRIG LOW
    PORTB &= ~(1 << TRIG_BIT);
    delayMicroseconds(2);

    // TRIG HIGH
    PORTB |= (1 << TRIG_BIT);
    delayMicroseconds(TRIGGER_PULSE_US);

    // TRIG LOW
    PORTB &= ~(1 << TRIG_BIT);
}

// Return the duration in which echo pin stays high in microseconds
static unsigned long measureEchoPulse() {
    unsigned long start;
    unsigned long begin = micros(); // End of the 10 microseconds for triggering

    // Wait for ECHO to go HIGH
    while (!(PINB & (1 << ECHO_BIT))) {
        if (micros() - begin >= MAX_ECHO_US) return 0;  // timeout
    }

    // ECHO is now HIGH, start timing
    start = micros(); // Actual start when echo is set high

    // Wait for ECHO to go LOW
    while (PINB & (1 << ECHO_BIT)) {
        if (micros() - start >= MAX_ECHO_US) return 0;  // timeout
    }

    // Return how long ECHO stayed HIGH
    return micros() - start;
}

void ultrasonicBegin() {
    // Set TRIG pin as OUTPUT
    DDRB |= (1 << TRIG_BIT);

    // Set ECHO pin as INPUT
    DDRB &= ~(1 << ECHO_BIT);

    // Make sure TRIG starts LOW
    PORTB &= ~(1 << TRIG_BIT);

    delay(MIN_STARTUP_DELAY_MS);
}

unsigned long ultrasonicReadRawPulse() {
    sendTrigger();
    return measureEchoPulse();
}

float ultrasonicReadDistanceCm() {
    unsigned long time = ultrasonicReadRawPulse();
    if (time == 0) return -1.0f;
    return (time * SOUND_SPEED_CM_US) / 2.0f;
}