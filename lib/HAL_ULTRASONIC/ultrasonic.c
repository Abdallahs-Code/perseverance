#include "ultrasonic.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMEOUT_TICKS  80000U    // 40ms @ prescaler 8, 16MHz

// Sensor instances
UltrasonicSensor sensor_front;
UltrasonicSensor sensor_left;
UltrasonicSensor sensor_right;

// Disarms both OCR compare and ICU, marks sensor done
static inline void sensor_finish(UltrasonicSensor   *s,
                                  float               dist,
                                  volatile uint8_t   *timsk,
                                  uint8_t             ocie_bit,
                                  uint8_t             icie_bit)
{
    *timsk       &= ~(1 << ocie_bit);
    *timsk       &= ~(1 << icie_bit);
    s->edge_state = 0;
    s->distance   = dist;
    s->ready      = 1;
}

// Timer3 ISRs
ISR(TIMER3_CAPT_vect)
{
    if (sensor_front.edge_state == 0) {
        sensor_front.rising_time = ICR3;
        sensor_front.edge_state  = 1;

        OCR3A  = (uint16_t)(ICR3 + TIMEOUT_TICKS);
        TIFR3 |= (1 << OCF3A);
        TIMSK3 |= (1 << OCIE3A);

        TCCR3B &= ~(1 << ICES3);   // switch to falling edge
        TIFR3  |=  (1 << ICF3);    // clear stale capture flag
    } else {
        float dist = ((ICR3 - sensor_front.rising_time) * 0.5f) / 58.0f;
        sensor_finish(&sensor_front, dist, &TIMSK3, OCIE3A, ICIE3);
    }
}

ISR(TIMER3_COMPA_vect)
{
    sensor_finish(&sensor_front, ULTRASONIC_NO_ECHO, &TIMSK3, OCIE3A, ICIE3);
}

// Timer4 ISRs
ISR(TIMER4_CAPT_vect)
{
    if (sensor_left.edge_state == 0) {
        sensor_left.rising_time = ICR4;
        sensor_left.edge_state  = 1;

        OCR4A  = (uint16_t)(ICR4 + TIMEOUT_TICKS);
        TIFR4 |= (1 << OCF4A);
        TIMSK4 |= (1 << OCIE4A);

        TCCR4B &= ~(1 << ICES4);
        TIFR4  |=  (1 << ICF4);
    } else {
        float dist = ((ICR4 - sensor_left.rising_time) * 0.5f) / 58.0f;
        sensor_finish(&sensor_left, dist, &TIMSK4, OCIE4A, ICIE4);
    }
}

ISR(TIMER4_COMPA_vect)
{
    sensor_finish(&sensor_left, ULTRASONIC_NO_ECHO, &TIMSK4, OCIE4A, ICIE4);
}

// Timer5 ISRs
ISR(TIMER5_CAPT_vect)
{
    if (sensor_right.edge_state == 0) {
        sensor_right.rising_time = ICR5;
        sensor_right.edge_state  = 1;

        OCR5A  = (uint16_t)(ICR5 + TIMEOUT_TICKS);
        TIFR5 |= (1 << OCF5A);
        TIMSK5 |= (1 << OCIE5A);

        TCCR5B &= ~(1 << ICES5);
        TIFR5  |=  (1 << ICF5);
    } else {
        float dist = ((ICR5 - sensor_right.rising_time) * 0.5f) / 58.0f;
        sensor_finish(&sensor_right, dist, &TIMSK5, OCIE5A, ICIE5);
    }
}

ISR(TIMER5_COMPA_vect)
{
    sensor_finish(&sensor_right, ULTRASONIC_NO_ECHO, &TIMSK5, OCIE5A, ICIE5);
}

// Init — GPIO directions + timer config
void Ultrasonic_Init(void)
{
    // TRIG pins as output
    GPIO_SetPinDirection(TRIG_PORT, TRIG_FRONT_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(TRIG_PORT, TRIG_LEFT_PIN,  GPIO_OUTPUT);
    GPIO_SetPinDirection(TRIG_PORT, TRIG_RIGHT_PIN, GPIO_OUTPUT);

    // ECHO/ICP pins as input
    DDRE &= ~(1 << PE3);   // ICP3
    DDRL &= ~(1 << PL0);   // ICP4
    DDRL &= ~(1 << PL1);   // ICP5

    // Timer3 in normal mode, prescaler 8
    TCCR3A = 0x00;
    TCCR3B = (1 << CS31);

    TCCR4A = 0x00;
    TCCR4B = (1 << CS41);

    TCCR5A = 0x00;
    TCCR5B = (1 << CS51);
}

// Arm one sensor — sets edge direction, clears flags, enables ICU interrupt
static inline void arm_sensor(UltrasonicSensor  *s,
                               volatile uint16_t *tcnt,
                               volatile uint8_t  *tccr_b,
                               volatile uint8_t  *tifr,
                               volatile uint8_t  *timsk,
                               uint8_t            ices_bit,
                               uint8_t            icf_bit,
                               uint8_t            icie_bit)
{
    s->ready      = 0;
    s->edge_state = 0;
    *tcnt         = 0;
    *tccr_b      |= (1 << ices_bit);   // rising edge
    *tifr        |= (1 << icf_bit);    // clear stale flag
    *timsk       |= (1 << icie_bit);   // enable ICU interrupt
}

// Trigger all three simultaneously
void Ultrasonic_TriggerAll(void)
{
    arm_sensor(&sensor_front, &TCNT3, &TCCR3B, &TIFR3, &TIMSK3, ICES3, ICF3, ICIE3);
    arm_sensor(&sensor_left,  &TCNT4, &TCCR4B, &TIFR4, &TIMSK4, ICES4, ICF4, ICIE4);
    arm_sensor(&sensor_right, &TCNT5, &TCCR5B, &TIFR5, &TIMSK5, ICES5, ICF5, ICIE5);

    // Fire all three TRIG pins
    PORTE |= ((1 << TRIG_FRONT_PIN) | (1 << TRIG_LEFT_PIN) | (1 << TRIG_RIGHT_PIN));
    _delay_us(10);
    PORTE &= ~((1 << TRIG_FRONT_PIN) | (1 << TRIG_LEFT_PIN) | (1 << TRIG_RIGHT_PIN));
}

// Wait until all three sensors are ready (max 40ms)
void Ultrasonic_WaitAll(void)
{
    while (!sensor_front.ready || !sensor_left.ready || !sensor_right.ready);
}