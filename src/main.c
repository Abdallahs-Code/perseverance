#include <avr/io.h>
#include <util/delay.h>
#include "../lib/HAL_ULTRASONIC/ultrasonic.h"

/* ---------------------------------------------------------------
 * Simple UART init and print functions for ATmega2560
 * Baud rate: 9600
 * --------------------------------------------------------------- */
#define BAUD     9600
#define UBRR_VAL (F_CPU / 16 / BAUD - 1)

static void uart_init(void) {
    UBRR0H = (uint8_t)(UBRR_VAL >> 8);
    UBRR0L = (uint8_t)(UBRR_VAL);
    UCSR0B = (1 << TXEN0);                     /* Enable TX only */
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);    /* 8-bit, 1 stop  */
}

static void uart_putchar(char c) {
    while (!(UCSR0A & (1 << UDRE0)));          /* Wait for empty buffer */
    UDR0 = c;
}

static void uart_print(const char *str) {
    while (*str) uart_putchar(*str++);
}

/* Print a float with 1 decimal place */
static void uart_print_float(float val) {
    if (val == ULTRASONIC_TIMEOUT) {
        uart_print("TIMEOUT");
        return;
    }
    if (val == ULTRASONIC_NOT_READY) {
        uart_print("WAIT");
        return;
    }

    /* Handle negative (shouldn't happen for valid distances) */
    if (val < 0) { uart_putchar('-'); val = -val; }

    int integer = (int)val;
    int decimal = (int)((val - integer) * 10);

    /* Print integer part */
    if (integer == 0) {
        uart_putchar('0');
    } else {
        char buf[6];
        uint8_t idx = 0;
        while (integer > 0) {
            buf[idx++] = '0' + (integer % 10);
            integer /= 10;
        }
        while (idx--) uart_putchar(buf[idx]);  /* reverse */
    }

    uart_putchar('.');
    uart_putchar('0' + decimal);
}

int main(void) {
    uart_init();
    ultrasonicBegin();

    uart_print("Ultrasonic sensor test started\r\n");

    while (1) {
        /* Fire all 3 sensors simultaneously */
        ultrasonicTriggerAll();

        /* Wait long enough for echoes to return
         * Max range ~4m → ~23ms, we wait 60ms to be safe */
        _delay_ms(60);

        /* Read distances */
        float d1 = ultrasonicGetDistanceCm(ULTRASONIC_SENSOR_1);
        float d2 = ultrasonicGetDistanceCm(ULTRASONIC_SENSOR_2);
        float d3 = ultrasonicGetDistanceCm(ULTRASONIC_SENSOR_3);

        /* Print in requested format:
         * s1: {distance} cm  - s2: {distance} cm - s3: {distance} cm */
        uart_print("s1: "); uart_print_float(d1); uart_print(" cm");
        uart_print("  - ");
        uart_print("s2: "); uart_print_float(d2); uart_print(" cm");
        uart_print(" - ");
        uart_print("s3: "); uart_print_float(d3); uart_print(" cm");
        uart_print("\r\n");

        /* Wait before next measurement cycle
         * HC-SR04 datasheet recommends >60ms between triggers */
        _delay_ms(200);
    }

    return 0;
}