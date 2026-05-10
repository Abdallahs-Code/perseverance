#include <Arduino.h>
#include "gpio.h"
#include "pin_config.h"
#include "bluetooth.h"
#include "ultrasonic.h"
#include <util/delay.h>

int main(void)
{
    Serial.begin(9600);
    Ultrasonic_Init();
    sei();

    float distances[3];
    uint8_t results[3];

    while (1)
    {
        Ultrasonic_CheckSensors(distances, results);

        Serial.print(distances[0], 1); Serial.print(" ");
        Serial.print(distances[1], 1); Serial.print(" ");
        Serial.print(distances[2], 1); Serial.print(" ");
        Serial.print(results[0]); Serial.print(" ");
        Serial.print(results[1]); Serial.print(" ");
        Serial.println(results[2]);

        _delay_ms(20);
    }
}