#include "ultrasonic.h"
#include <avr/interrupt.h>
#include <util/delay.h>

/* ---------------------------------------------------------------
 * Internal state for each sensor
 * volatile because written inside ISRs, read in main code
 * --------------------------------------------------------------- */
typedef struct {
    volatile uint16_t timerStart;   /* TCNT3 on ECHO rising edge         */
    volatile uint16_t timerStop;    /* TCNT3 on ECHO falling edge        */
    volatile uint8_t  overflow;     /* Timer3 overflows during pulse     */
    volatile uint8_t  ready;        /* SENSOR_WAITING/READY/TIMEOUT      */
    volatile uint8_t  waitingFall;  /* 1 = rising seen, waiting for fall */
    volatile uint8_t  triggerSent;  /* 1 = trigger fired, echo not yet   */
    volatile uint8_t  waitOverflow; /* overflow count while waiting rise  */
} SensorState;

static volatile SensorState sensors[ULTRASONIC_SENSOR_COUNT];

/* ---------------------------------------------------------------
 * Timer3 overflow ISR
 *
 * Two jobs:
 *   1. Mid-pulse: sensor is between rising and falling edge
 *      → increment overflow so computePulseUs() stays accurate
 *
 *   2. Waiting for rising edge: trigger was sent but ECHO never
 *      went HIGH yet → increment waitOverflow and declare timeout
 *      if it exceeds TIMEOUT_OVERFLOWS
 * --------------------------------------------------------------- */
ISR(TIMER3_OVF_vect) {
    for (uint8 i = 0; i < ULTRASONIC_SENSOR_COUNT; i++) {
        if (sensors[i].waitingFall) {
            /* Job 1: mid-pulse, keep overflow count accurate */
            sensors[i].overflow++;
        } else if (sensors[i].triggerSent &&
                   sensors[i].ready == SENSOR_WAITING) {
            /* Job 2: waiting for ECHO rising edge — check timeout */
            sensors[i].waitOverflow++;
            if (sensors[i].waitOverflow >= TIMEOUT_OVERFLOWS) {
                sensors[i].ready       = SENSOR_TIMEOUT;
                sensors[i].triggerSent = 0;
            }
        }
    }
}

/* ---------------------------------------------------------------
 * Helper: compute pulse duration in microseconds
 * ticks = overflows * 65536 + (stop - start)
 * divide by TICKS_PER_US (2) to get microseconds
 * --------------------------------------------------------------- */
static unsigned long computePulseUs(volatile SensorState *s) {
    uint32_t ticks = ((uint32_t)s->overflow * 65536UL)
                   + (uint32_t)(s->timerStop - s->timerStart);
    return (unsigned long)(ticks / TICKS_PER_US);
}

/* ---------------------------------------------------------------
 * External Interrupt ISRs
 *
 * Each ISR handles both edges of the ECHO pulse:
 *   Rising  → record start time, switch interrupt to falling
 *   Falling → record stop time, mark ready, switch back to rising
 *
 * EICRA bits for each INTx:
 *   ISCx1=1 ISCx0=1 → rising edge
 *   ISCx1=1 ISCx0=0 → falling edge
 * --------------------------------------------------------------- */

ISR(INT0_vect) {
    if (!sensors[ULTRASONIC_SENSOR_1].waitingFall) {
        /* Rising edge — start timing */
        sensors[ULTRASONIC_SENSOR_1].timerStart  = TCNT3;
        sensors[ULTRASONIC_SENSOR_1].overflow    = 0;
        sensors[ULTRASONIC_SENSOR_1].ready       = SENSOR_WAITING;
        sensors[ULTRASONIC_SENSOR_1].waitingFall = 1;
        sensors[ULTRASONIC_SENSOR_1].triggerSent = 0; /* echo arrived, cancel timeout watch */
        /* Switch INT0 to falling edge */
        EICRA = (EICRA & ~((1 << ISC01) | (1 << ISC00))) | (1 << ISC01);
    } else {
        /* Falling edge — stop timing */
        sensors[ULTRASONIC_SENSOR_1].timerStop   = TCNT3;
        sensors[ULTRASONIC_SENSOR_1].waitingFall = 0;
        sensors[ULTRASONIC_SENSOR_1].ready       = SENSOR_READY;
        /* Switch INT0 back to rising edge */
        EICRA |= (1 << ISC01) | (1 << ISC00);
    }
}

ISR(INT1_vect) {
    if (!sensors[ULTRASONIC_SENSOR_2].waitingFall) {
        /* Rising edge */
        sensors[ULTRASONIC_SENSOR_2].timerStart  = TCNT3;
        sensors[ULTRASONIC_SENSOR_2].overflow    = 0;
        sensors[ULTRASONIC_SENSOR_2].ready       = SENSOR_WAITING;
        sensors[ULTRASONIC_SENSOR_2].waitingFall = 1;
        sensors[ULTRASONIC_SENSOR_2].triggerSent = 0;
        /* Switch INT1 to falling edge */
        EICRA = (EICRA & ~((1 << ISC11) | (1 << ISC10))) | (1 << ISC11);
    } else {
        /* Falling edge */
        sensors[ULTRASONIC_SENSOR_2].timerStop   = TCNT3;
        sensors[ULTRASONIC_SENSOR_2].waitingFall = 0;
        sensors[ULTRASONIC_SENSOR_2].ready       = SENSOR_READY;
        /* Switch INT1 back to rising edge */
        EICRA |= (1 << ISC11) | (1 << ISC10);
    }
}

ISR(INT2_vect) {
    if (!sensors[ULTRASONIC_SENSOR_3].waitingFall) {
        /* Rising edge */
        sensors[ULTRASONIC_SENSOR_3].timerStart  = TCNT3;
        sensors[ULTRASONIC_SENSOR_3].overflow    = 0;
        sensors[ULTRASONIC_SENSOR_3].ready       = SENSOR_WAITING;
        sensors[ULTRASONIC_SENSOR_3].waitingFall = 1;
        sensors[ULTRASONIC_SENSOR_3].triggerSent = 0;
        /* Switch INT2 to falling edge */
        EICRA = (EICRA & ~((1 << ISC21) | (1 << ISC20))) | (1 << ISC21);
    } else {
        /* Falling edge */
        sensors[ULTRASONIC_SENSOR_3].timerStop   = TCNT3;
        sensors[ULTRASONIC_SENSOR_3].waitingFall = 0;
        sensors[ULTRASONIC_SENSOR_3].ready       = SENSOR_READY;
        /* Switch INT2 back to rising edge */
        EICRA |= (1 << ISC21) | (1 << ISC20);
    }
}

/* ---------------------------------------------------------------
 * ultrasonicBegin()
 * --------------------------------------------------------------- */
void ultrasonicBegin(void) {
    /* --- TRIG pins: output, start LOW --- */
    GPIO_SetPinDirection(TRIG1_PORT, TRIG1_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(TRIG2_PORT, TRIG2_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(TRIG3_PORT, TRIG3_PIN, GPIO_OUTPUT);
    GPIO_WritePin(TRIG1_PORT, TRIG1_PIN, GPIO_LOW);
    GPIO_WritePin(TRIG2_PORT, TRIG2_PIN, GPIO_LOW);
    GPIO_WritePin(TRIG3_PORT, TRIG3_PIN, GPIO_LOW);

    /* --- ECHO pins: input --- */
    GPIO_SetPinDirection(ECHO1_PORT, ECHO1_PIN, GPIO_INPUT);
    GPIO_SetPinDirection(ECHO2_PORT, ECHO2_PIN, GPIO_INPUT);
    GPIO_SetPinDirection(ECHO3_PORT, ECHO3_PIN, GPIO_INPUT);

    /* --- Timer3: normal mode, prescaler=8, enable overflow IRQ --- */
    TCCR3A = 0x00;
    TCCR3B = TIMER3_PRESCALER_BITS;
    TCNT3  = 0;
    TIMSK3 |= (1 << TOIE3);

    /* --- External interrupts: all start on RISING edge --- */
    EICRA |= (1 << ISC01) | (1 << ISC00)   /* INT0 rising */
           | (1 << ISC11) | (1 << ISC10)   /* INT1 rising */
           | (1 << ISC21) | (1 << ISC20);  /* INT2 rising */
    EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT2);

    /* --- Clear sensor state --- */
    for (uint8 i = 0; i < ULTRASONIC_SENSOR_COUNT; i++) {
        sensors[i].timerStart  = 0;
        sensors[i].timerStop   = 0;
        sensors[i].overflow    = 0;
        sensors[i].ready       = SENSOR_WAITING;
        sensors[i].waitingFall = 0;
        sensors[i].triggerSent = 0;
        sensors[i].waitOverflow = 0;
    }

    sei();
    _delay_ms(MIN_STARTUP_DELAY_MS);
}

/* ---------------------------------------------------------------
 * ultrasonicTriggerAll()
 * Sends a 10us pulse on all 3 TRIG pins simultaneously
 * Also arms the timeout watchdog for each sensor
 * --------------------------------------------------------------- */
void ultrasonicTriggerAll(void) {
    /* Arm timeout watchdog for all sensors */
    for (uint8 i = 0; i < ULTRASONIC_SENSOR_COUNT; i++) {
        sensors[i].ready       = SENSOR_WAITING;
        sensors[i].triggerSent = 1;
        sensors[i].waitOverflow = 0;
    }

    /* All LOW first */
    GPIO_WritePin(TRIG1_PORT, TRIG1_PIN, GPIO_LOW);
    GPIO_WritePin(TRIG2_PORT, TRIG2_PIN, GPIO_LOW);
    GPIO_WritePin(TRIG3_PORT, TRIG3_PIN, GPIO_LOW);
    _delay_us(2);

    /* All HIGH simultaneously */
    GPIO_WritePin(TRIG1_PORT, TRIG1_PIN, GPIO_HIGH);
    GPIO_WritePin(TRIG2_PORT, TRIG2_PIN, GPIO_HIGH);
    GPIO_WritePin(TRIG3_PORT, TRIG3_PIN, GPIO_HIGH);
    _delay_us(TRIGGER_PULSE_US);

    /* All LOW simultaneously */
    GPIO_WritePin(TRIG1_PORT, TRIG1_PIN, GPIO_LOW);
    GPIO_WritePin(TRIG2_PORT, TRIG2_PIN, GPIO_LOW);
    GPIO_WritePin(TRIG3_PORT, TRIG3_PIN, GPIO_LOW);
}

/* ---------------------------------------------------------------
 * ultrasonicIsReady()
 * Returns the raw state code: SENSOR_WAITING, SENSOR_READY,
 * or SENSOR_TIMEOUT — useful if caller wants to distinguish
 * --------------------------------------------------------------- */
uint8 ultrasonicIsReady(uint8 sensorIndex) {
    if (sensorIndex >= ULTRASONIC_SENSOR_COUNT) return SENSOR_WAITING;
    return sensors[sensorIndex].ready;
}

/* ---------------------------------------------------------------
 * ultrasonicGetRawPulse()
 * Returns pulse duration in microseconds.
 * Returns 0 if not ready or timed out.
 * --------------------------------------------------------------- */
unsigned long ultrasonicGetRawPulse(uint8 sensorIndex) {
    if (sensorIndex >= ULTRASONIC_SENSOR_COUNT)          return 0;
    if (sensors[sensorIndex].ready != SENSOR_READY)      return 0;
    return computePulseUs(&sensors[sensorIndex]);
}

/* ---------------------------------------------------------------
 * ultrasonicGetDistanceCm()
 *   Returns distance in cm       on success
 *   Returns ULTRASONIC_TIMEOUT   (-1.0) on timeout
 *   Returns ULTRASONIC_NOT_READY (-2.0) if still waiting
 * --------------------------------------------------------------- */
float ultrasonicGetDistanceCm(uint8 sensorIndex) {
    if (sensorIndex >= ULTRASONIC_SENSOR_COUNT)
        return ULTRASONIC_TIMEOUT;

    uint8 state = sensors[sensorIndex].ready;

    if (state == SENSOR_TIMEOUT)  return ULTRASONIC_TIMEOUT;
    if (state == SENSOR_WAITING)  return ULTRASONIC_NOT_READY;

    unsigned long pulseUs = computePulseUs(&sensors[sensorIndex]);
    return (pulseUs * SOUND_SPEED_CM_US) / 2.0f;
}