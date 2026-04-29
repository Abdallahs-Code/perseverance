#include "timer.h"
#include <avr/io.h>

void Timer1_PWM_Init(void)
{
    // Set OC1A (PB1) and OC1B (PB2) as output
    DDRB |= (1 << 1) | (1 << 2);

    // Fast PWM Mode 14: WGM13:0 = 14 (1110)
    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    // Non-inverting mode
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1);

    // Set TOP value for frequency
    ICR1 = 199;

    // Start timer with prescaler = 8
    TCCR1B |= (1 << CS11);
}


void Timer1_SetDuty(uint8 channel, uint8 duty)
{
    if(duty > 100) duty = 100;

    uint16 value = (duty * 199) / 100;

    switch(channel)
    {
        case TIMER1_CHANNEL_A:
            OCR1A = value;
            break;

        case TIMER1_CHANNEL_B:
            OCR1B = value;
            break;
    }
}