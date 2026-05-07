#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../lib/HAL_ULTRASONIC/ultrasonic.h"

/* ---------------------------------------------------------------
 * Redirect stdout → UART0 so printf works in PlatformIO monitor
 * Baud: 9600, F_CPU must be defined (set in platformio.ini)
 * --------------------------------------------------------------- */
static int uart_putchar(char c, FILE *stream) {
    if (c == '\n') uart_putchar('\r', stream);
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
    return 0;
}

static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static void uart_init(void) {
#define BAUD 9600
    uint16_t ubrr = F_CPU / 16 / BAUD - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);
    UCSR0B = (1 << TXEN0);                      /* TX only, no RX needed */
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);    /* 8-bit, 1 stop, no parity */
    stdout = &uart_stdout;
}

int main(void) {
    uart_init();
    ultrasonicBegin();

    float distances[ULTRASONIC_SENSOR_COUNT];

    while (1) {
        ultrasonicReadAllCm(distances, MAX_ECHO_US);

        /* Multiply by 10, print as integer with manual decimal point */
        int s1 = (int)(distances[0] * 10);
        int s2 = (int)(distances[1] * 10);
        int s3 = (int)(distances[2] * 10);

        printf("s1: %d.%d cm - s2: %d.%d cm - s3: %d.%d cm\n",
            s1 / 10, s1 % 10,
            s2 / 10, s2 % 10,
            s3 / 10, s3 % 10);

        _delay_ms(500);
    }
}