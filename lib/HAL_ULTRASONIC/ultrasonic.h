#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "../CORE/std_types.h"
#include "../MCAL_GPIO/gpio.h"
#include <avr/io.h>

/* ---------------------------------------------------------------
 * Number of sensors
 * --------------------------------------------------------------- */
#define ULTRASONIC_SENSOR_COUNT  3

/* ---------------------------------------------------------------
 * Sensor index aliases
 * --------------------------------------------------------------- */
#define ULTRASONIC_SENSOR_1      0
#define ULTRASONIC_SENSOR_2      1
#define ULTRASONIC_SENSOR_3      2

/* ---------------------------------------------------------------
 * TRIG pins — any free GPIO output pins
 *   Sensor 1 TRIG: Port F, Pin 0  (Arduino Mega pin A0 / pin 54)
 *   Sensor 2 TRIG: Port A, Pin 1  (Arduino Mega pin 23) - unused
 *   Sensor 3 TRIG: Port A, Pin 2  (Arduino Mega pin 24) - unused
 * --------------------------------------------------------------- */
#define TRIG1_PORT   PORT_F
#define TRIG1_PIN    0

#define TRIG2_PORT   PORT_A
#define TRIG2_PIN    1

#define TRIG3_PORT   PORT_A
#define TRIG3_PIN    2

/* ---------------------------------------------------------------
 * ECHO pins — MUST be external interrupt pins on ATmega2560
 *   Sensor 1 ECHO: Arduino Mega pin 21 → INT0 → Port D, Pin 0
 *   Sensor 2 ECHO: Arduino Mega pin 3  → INT1 → Port D, Pin 1
 *   Sensor 3 ECHO: Arduino Mega pin 21 → INT2 → Port D, Pin 2
 * --------------------------------------------------------------- */
#define ECHO1_PORT   PORT_D
#define ECHO1_PIN    0   /* INT0 */

#define ECHO2_PORT   PORT_D
#define ECHO2_PIN    1   /* INT1 */

#define ECHO3_PORT   PORT_D
#define ECHO3_PIN    2   /* INT2 */

/* ---------------------------------------------------------------
 * Timer3 configuration (16-bit, ATmega2560)
 *   Prescaler = 8  →  tick = 0.5 µs  →  overflow every ~32 ms
 * --------------------------------------------------------------- */
#define TIMER3_PRESCALER_BITS    (1 << CS31)   /* prescaler = 8 */
#define TICKS_PER_US             2UL            /* 2 ticks = 1 µs */

/* ---------------------------------------------------------------
 * Timing constants
 * --------------------------------------------------------------- */
#define SOUND_SPEED_CM_US        0.0343f
#define TRIGGER_PULSE_US         10
#define MIN_STARTUP_DELAY_MS     60
#define MAX_ECHO_US              30000UL

/* ---------------------------------------------------------------
 * Timeout detection
 *   Timer3 overflows every 128µs (prescaler=8, 16-bit at 16MHz
 *   — wait, Timer3 is 16-bit so overflow = 65536 ticks × 0.5µs
 *   = 32,768µs ≈ 32ms per overflow)
 *   MAX_ECHO_US / 32768µs ≈ 1 overflow covers full range.
 *   We use 2 overflows as a safe timeout margin.
 * --------------------------------------------------------------- */
#define TIMEOUT_OVERFLOWS        2

/* ---------------------------------------------------------------
 * Sensor ready state codes
 *   SENSOR_WAITING  — trigger sent, no echo yet
 *   SENSOR_READY    — echo received, data valid
 *   SENSOR_TIMEOUT  — echo never arrived within timeout window
 * --------------------------------------------------------------- */
#define SENSOR_WAITING           0
#define SENSOR_READY             1
#define SENSOR_TIMEOUT           2

/* ---------------------------------------------------------------
 * Return values for ultrasonicGetDistanceCm()
 *   -1.0 → timeout  (no echo received)
 *   -2.0 → not ready yet (still waiting)
 * --------------------------------------------------------------- */
#define ULTRASONIC_TIMEOUT       -1.0f
#define ULTRASONIC_NOT_READY     -2.0f

#ifdef __cplusplus
extern "C" {
#endif

void ultrasonicBegin(void);
void ultrasonicTriggerAll(void);
unsigned long ultrasonicGetRawPulse(uint8 sensorIndex);
float ultrasonicGetDistanceCm(uint8 sensorIndex);
uint8 ultrasonicIsReady(uint8 sensorIndex);

#ifdef __cplusplus
}
#endif

#endif /* ULTRASONIC_H */