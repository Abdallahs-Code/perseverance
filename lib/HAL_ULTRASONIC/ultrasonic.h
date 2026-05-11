#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "../CORE/std_types.h"
#include "../MCAL_GPIO/gpio.h"
#include <avr/io.h>

// Pin configuration
#define TRIG_PORT       PORT_A
#define TRIG_PIN_0      4       // Arduino Mega D26
#define TRIG_PIN_1      5       // Arduino Mega D27
#define TRIG_PIN_2      6       // Arduino Mega D28

#define ECHO_PORT       PORT_H
#define ECHO_PIN_0      3       // Arduino Mega D6
#define ECHO_PIN_1      4       // Arduino Mega D7
#define ECHO_PIN_2      5       // Arduino Mega D8

#define ULTRASONIC_SENSOR_COUNT     3

// Timer3 configuration
// Prescaler = 8  →  1 tick = 0.5µs
// Max echo 30,000µs = 60,000 ticks - fits in 16-bit counter
#define TICKS_PER_US                2UL     // 2 ticks = 1µs

// Timing constants
#define SOUND_SPEED_CM_US           0.0343f
#define TRIGGER_PULSE_US            10
#define MIN_STARTUP_DELAY_MS        60
#define MAX_ECHO_US                 30000UL // ~5m range cap
#define INTER_SENSOR_DELAY_MS       5       // settling between sensors

// Return sentinels
//   -1.0f  timeout (echo never arrived or pulse too long)
#define ULTRASONIC_TIMEOUT          -1.0f

// Per-sensor distance thresholds in cm
#define ULTRASONIC_THRESHOLD_0      45.0f   // Sensor 0
#define ULTRASONIC_THRESHOLD_1      25.0f   // Sensor 1
#define ULTRASONIC_THRESHOLD_2      45.0f   // Sensor 2

// Threshold check return codes
#define ULTRASONIC_BELOW_THRESHOLD  1
#define ULTRASONIC_ABOVE_THRESHOLD  0

#ifdef __cplusplus
extern "C" {
#endif

// Initialize Timer3 and all TRIG/ECHO pins
void ultrasonicBegin(void);

void ultrasonicReadAllCm(float results[ULTRASONIC_SENSOR_COUNT], uint32 timeoutUs);

// Check each sensor reading against its threshold.
void ultrasonicCheckThresholds(float distances[ULTRASONIC_SENSOR_COUNT], uint8 results[ULTRASONIC_SENSOR_COUNT]);

#ifdef __cplusplus
}
#endif

#endif