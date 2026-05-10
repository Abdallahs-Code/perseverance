#include "ultrasonic.h"
#include "../MCAL_TIMER/timer.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMEOUT_TICKS  60000U    // ~30 ms @ prescaler 8, 16 MHz (HC-SR04 max echo ~25 ms)

UltrasonicSensor sensor_front;
UltrasonicSensor sensor_left;
UltrasonicSensor sensor_right;

// ---- Front sensor: INT2 edge detection + Timer3 free-running counter ----

static inline void front_finish(float dist) {
    EIMSK  &= ~(1 << INT2);
    TIMSK3 &= ~(1 << OCIE3A);
    sensor_front.edge_state = 0;
    sensor_front.distance   = dist;
    sensor_front.ready      = 1;
}

ISR(INT2_vect) {
    uint16_t now = TCNT3;
    if (sensor_front.edge_state == 0) {
        sensor_front.rising_time = now;
        sensor_front.edge_state  = 1;
    } else {
        float dist = ((uint16_t)(now - sensor_front.rising_time) * 0.5f) / 58.0f;
        front_finish(dist);
    }
}

ISR(TIMER3_COMPA_vect) {
    front_finish(ULTRASONIC_NO_ECHO);
}

// ---- Left/Right sensors: ICU on Timer4/Timer5 ----

static inline void icu_finish(UltrasonicSensor *s,
                              float             dist,
                              volatile uint8_t *timsk,
                              uint8_t           ocie_bit,
                              uint8_t           icie_bit)
{
    *timsk       &= ~(1 << ocie_bit);
    *timsk       &= ~(1 << icie_bit);
    s->edge_state = 0;
    s->distance   = dist;
    s->ready      = 1;
}

ISR(TIMER4_CAPT_vect) {
    if (sensor_left.edge_state == 0) {
        sensor_left.rising_time = ICR4;
        sensor_left.edge_state  = 1;
        TCCR4B &= ~(1 << ICES4);   // switch to falling edge
        TIFR4  |=  (1 << ICF4);
    } else {
        float dist = ((uint16_t)(ICR4 - sensor_left.rising_time) * 0.5f) / 58.0f;
        icu_finish(&sensor_left, dist, &TIMSK4, OCIE4A, ICIE4);
    }
}

ISR(TIMER4_COMPA_vect) {
    icu_finish(&sensor_left, ULTRASONIC_NO_ECHO, &TIMSK4, OCIE4A, ICIE4);
}

ISR(TIMER5_CAPT_vect) {
    if (sensor_right.edge_state == 0) {
        sensor_right.rising_time = ICR5;
        sensor_right.edge_state  = 1;
        TCCR5B &= ~(1 << ICES5);
        TIFR5  |=  (1 << ICF5);
    } else {
        float dist = ((uint16_t)(ICR5 - sensor_right.rising_time) * 0.5f) / 58.0f;
        icu_finish(&sensor_right, dist, &TIMSK5, OCIE5A, ICIE5);
    }
}

ISR(TIMER5_COMPA_vect) {
    icu_finish(&sensor_right, ULTRASONIC_NO_ECHO, &TIMSK5, OCIE5A, ICIE5);
}

void Ultrasonic_Init(void) {
    // TRIG pins as output
    DDRE |= (1 << TRIG_FRONT_PIN) | (1 << TRIG_LEFT_PIN);   // PE4, PE5
    DDRH |= (1 << TRIG_RIGHT_PIN);                          // PH4

    // ECHO pins as input
    DDRD &= ~(1 << PD2);   // INT2 (front)
    DDRL &= ~(1 << PL0);   // ICP4 (left)
    DDRL &= ~(1 << PL1);   // ICP5 (right)

    // INT2: trigger on any logical change (both edges)
    EICRA = (EICRA & ~((1 << ISC21) | (1 << ISC20))) | (1 << ISC20);

    // Timers 3/4/5: normal mode, prescaler 8 (free-running)
    TCCR3A = 0x00;  TCCR3B = (1 << CS31);
    TCCR4A = 0x00;  TCCR4B = (1 << CS41);
    TCCR5A = 0x00;  TCCR5B = (1 << CS51);
}

// Arm the two ICU sensors: clear counter, select rising edge, enable ICU + timeout
static inline void arm_icu(UltrasonicSensor  *s,
                           volatile uint16_t *tcnt,
                           volatile uint16_t *ocr,
                           volatile uint8_t  *tccr_b,
                           volatile uint8_t  *tifr,
                           volatile uint8_t  *timsk,
                           uint8_t            ices_bit,
                           uint8_t            icf_bit,
                           uint8_t            ocf_bit,
                           uint8_t            icie_bit,
                           uint8_t            ocie_bit)
{
    s->ready      = 0;
    s->edge_state = 0;
    *tcnt         = 0;
    *ocr          = TIMEOUT_TICKS;
    *tccr_b      |= (1 << ices_bit);
    *tifr        |= (1 << icf_bit) | (1 << ocf_bit);
    *timsk       |= (1 << icie_bit) | (1 << ocie_bit);
}

// Arm the front sensor: Timer3 free-running counter + INT2 edge interrupt
static inline void arm_front(void) {
    sensor_front.ready      = 0;
    sensor_front.edge_state = 0;
    TCNT3  = 0;
    OCR3A  = TIMEOUT_TICKS;
    EIFR  |= (1 << INTF2);
    TIFR3 |= (1 << OCF3A);
    EIMSK |= (1 << INT2);
    TIMSK3 |= (1 << OCIE3A);
}

void ultrasonic_trigger_all(void) {
    arm_front();
    arm_icu(&sensor_left,  &TCNT4, &OCR4A, &TCCR4B, &TIFR4, &TIMSK4,
            ICES4, ICF4, OCF4A, ICIE4, OCIE4A);
    arm_icu(&sensor_right, &TCNT5, &OCR5A, &TCCR5B, &TIFR5, &TIMSK5,
            ICES5, ICF5, OCF5A, ICIE5, OCIE5A);

    // Fire all three TRIGs simultaneously (front+left on PORTE, right on PORTH)
    PORTE |= (1 << TRIG_FRONT_PIN) | (1 << TRIG_LEFT_PIN);
    PORTH |= (1 << TRIG_RIGHT_PIN);
    _delay_us(10);
    PORTE &= ~((1 << TRIG_FRONT_PIN) | (1 << TRIG_LEFT_PIN));
    PORTH &= ~(1 << TRIG_RIGHT_PIN);
}

void ultrasonic_wait_all(void) {
    while (!sensor_front.ready || !sensor_left.ready || !sensor_right.ready);
}

float *ultrasonic_get_distances(void) {
    ultrasonic_trigger_all();
    ultrasonic_wait_all();

    static float distances[3];

    distances[0] = sensor_front.distance;
    distances[1] = sensor_left.distance;
    distances[2] = sensor_right.distance;

    return distances;
}

void Ultrasonic_CheckSensors(float distances[3], uint8 results[3]) {
    float *dists = ultrasonic_get_distances();

    for (uint8 i = 0; i < 3; i++) {
        if (dists[i] == -1.0f) {
            distances[i] = 0.0f;
        }
        else {
            distances[i] = dists[i];
        }
    }

    if (dists[0] == -1.0f) {
        results[0] = ULTRASONIC_ABOVE_THRESHOLD;
    } else {
        results[0] = (dists[0] <= ULTRASONIC_THRESHOLD_FRONT) 
                            ? ULTRASONIC_BELOW_THRESHOLD
                            : ULTRASONIC_ABOVE_THRESHOLD;
    }

    if (dists[1] == -1.0f) {
        results[1] = ULTRASONIC_ABOVE_THRESHOLD;
    } else {
        results[1] = (dists[1] <= ULTRASONIC_THRESHOLD_LEFT) 
                            ? ULTRASONIC_BELOW_THRESHOLD
                            : ULTRASONIC_ABOVE_THRESHOLD;
    }

    if (dists[2] == -1.0f) {
        results[2] = ULTRASONIC_ABOVE_THRESHOLD;
    } else {
        results[2] = (dists[2] <= ULTRASONIC_THRESHOLD_RIGHT) 
                            ? ULTRASONIC_BELOW_THRESHOLD
                            : ULTRASONIC_ABOVE_THRESHOLD;
    }
}