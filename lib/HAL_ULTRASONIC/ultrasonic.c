#include "ultrasonic.h"
#include "../MCAL_TIMER/timer.h"
#include <util/delay.h>

/* ---------------------------------------------------------------
 * Pin lookup tables — indexed by sensor number
 * --------------------------------------------------------------- */
static const uint8 TRIG_PINS[ULTRASONIC_SENSOR_COUNT] = {
    TRIG_PIN_0,
    TRIG_PIN_1,
    TRIG_PIN_2
};

static const uint8 ECHO_PINS[ULTRASONIC_SENSOR_COUNT] = {
    ECHO_PIN_0,
    ECHO_PIN_1,
    ECHO_PIN_2
};

/* ---------------------------------------------------------------
 * Per-sensor thresholds — mirrors the constants in ultrasonic.h
 * --------------------------------------------------------------- */
static const float THRESHOLDS[ULTRASONIC_SENSOR_COUNT] = {
    ULTRASONIC_THRESHOLD_0,
    ULTRASONIC_THRESHOLD_1,
    ULTRASONIC_THRESHOLD_2
};

/* ---------------------------------------------------------------
 * sendTrigger()
 * Fires the 10µs TRIG pulse for one sensor
 * --------------------------------------------------------------- */
static void sendTrigger(uint8 sensorIndex) {
    uint8 pin = TRIG_PINS[sensorIndex];

    GPIO_WritePin(TRIG_PORT, pin, GPIO_LOW);
    _delay_us(2);
    GPIO_WritePin(TRIG_PORT, pin, GPIO_HIGH);
    _delay_us(TRIGGER_PULSE_US);
    GPIO_WritePin(TRIG_PORT, pin, GPIO_LOW);
}

/* ---------------------------------------------------------------
 * measurePulseUs()
 * Waits for ECHO to go HIGH then LOW, returns pulse width in µs.
 * Returns 0 on timeout (waiting for rise or fall).
 * --------------------------------------------------------------- */
static unsigned long measurePulseUs(uint8 sensorIndex, uint32 timeoutUs) {
    uint8  pin          = ECHO_PINS[sensorIndex];
    uint16 timeoutTicks = (uint16)(timeoutUs * TICKS_PER_US);
    uint16 startWait;
    uint16 startPulse;

    /* Wait for ECHO to go HIGH */
    startWait = Timer3_Now();
    while (GPIO_ReadPin(ECHO_PORT, pin) == GPIO_LOW) {
        if (Timer3_Elapsed(startWait) >= timeoutTicks) return 0;
    }

    /* ECHO is HIGH — start measuring */
    startPulse = Timer3_Now();

    /* Wait for ECHO to go LOW */
    while (GPIO_ReadPin(ECHO_PORT, pin) == GPIO_HIGH) {
        if (Timer3_Elapsed(startPulse) >= timeoutTicks) return 0;
    }

    /* Convert ticks to microseconds */
    return (unsigned long)(Timer3_Elapsed(startPulse) / TICKS_PER_US);
}

/* ---------------------------------------------------------------
 * ultrasonicBegin()
 * --------------------------------------------------------------- */
void ultrasonicBegin(void) {
    Timer3_Init();

    for (uint8 i = 0; i < ULTRASONIC_SENSOR_COUNT; i++) {
        GPIO_SetPinDirection(TRIG_PORT, TRIG_PINS[i], GPIO_OUTPUT);
        GPIO_SetPinDirection(ECHO_PORT, ECHO_PINS[i], GPIO_INPUT);
        GPIO_WritePin(TRIG_PORT, TRIG_PINS[i], GPIO_LOW);
    }

    _delay_ms(MIN_STARTUP_DELAY_MS);
}

/* ---------------------------------------------------------------
 * ultrasonicReadAllCm()
 * Reads all sensors sequentially. Fills results[] with distance
 * in cm, or ULTRASONIC_TIMEOUT (-1.0f) if a sensor timed out.
 * --------------------------------------------------------------- */
void ultrasonicReadAllCm(float results[ULTRASONIC_SENSOR_COUNT], uint32 timeoutUs) {
    for (uint8 i = 0; i < ULTRASONIC_SENSOR_COUNT; i++) {
        sendTrigger(i);

        unsigned long pulseUs = measurePulseUs(i, timeoutUs);

        if (pulseUs == 0) {
            results[i] = ULTRASONIC_TIMEOUT;
        } else {
            results[i] = (pulseUs * SOUND_SPEED_CM_US) / 2.0f;
        }

        /* Settling time — let ECHO line go idle before next sensor */
        if (i < ULTRASONIC_SENSOR_COUNT - 1) {
            _delay_ms(INTER_SENSOR_DELAY_MS);
        }
    }
}

/* ---------------------------------------------------------------
 * ultrasonicCheckThresholds()
 * Compares each distance against its per-sensor threshold.
 *   1 (ULTRASONIC_BELOW_THRESHOLD) — object at or closer than threshold
 *   0 (ULTRASONIC_ABOVE_THRESHOLD) — clear or timeout
 * --------------------------------------------------------------- */
void ultrasonicCheckThresholds(float distances[ULTRASONIC_SENSOR_COUNT],
                                uint8 results[ULTRASONIC_SENSOR_COUNT]) {
    for (uint8 i = 0; i < ULTRASONIC_SENSOR_COUNT; i++) {
        if (distances[i] == ULTRASONIC_TIMEOUT) {
            results[i] = ULTRASONIC_ABOVE_THRESHOLD;
        } else {
            results[i] = (distances[i] <= THRESHOLDS[i])
                             ? ULTRASONIC_BELOW_THRESHOLD
                             : ULTRASONIC_ABOVE_THRESHOLD;
        }
    }
}