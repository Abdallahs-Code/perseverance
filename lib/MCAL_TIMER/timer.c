#include "timer.h"
#include <avr/io.h>


// Timer 1 - Motors PWM
void Timer1_PWM_Init(void)
{
    // OC1A (PB5 = D11), OC1B (PB6 = D12)
    DDRB |= (1 << 5) | (1 << 6);

    // Fast PWM mode 14 (TOP = ICR1)
    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    // Non-inverting mode
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1);

    // Frequency setup (~10kHz)
    /*
    Frequency Calculation

    Clock = 16 MHz, Prescaler = 8
    Formula:
    fPWM = 16x10^6 / 8 x (1 + 199) = 10 kHz
    */
    ICR1 = 199;

    // Prescaler = 8
    TCCR1B |= (1 << CS11);
}

void Timer1_SetDuty(uint8 channel, uint8 duty)
{
    if(duty > 100) duty = 100;

    uint16 value = (duty * 199) / 100;

    switch(channel)
    {
        case TIMER1_CHANNEL_A:
            OCR1A = value; // D11
            break;

        case TIMER1_CHANNEL_B:
            OCR1B = value; // D12
            break;
    }
}

// Timer3 helpers
void Timer3_Init(void) {
    TCCR3A = 0x00;
    TCCR3B = (1 << CS31);   // Prescaler = 8, normal mode
    TCNT3  = 0; // Initial value
}

uint16 Timer3_Now(void) {
    return TCNT3;
}

uint16 Timer3_Elapsed(uint16 start) {
    return (uint16)(TCNT3 - start);
}