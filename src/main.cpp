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

    uint8_t results[3];

    while (1)
    {
        Serial.print("Start measurement... ");
        Ultrasonic_CheckThresholds(results);

        float f = sensor_front.distance;
        float l = sensor_left.distance;
        float r = sensor_right.distance;

        Serial.print("Front: "); Serial.print(results[0]);
        Serial.print(" | Left: "); Serial.print(results[1]);
        Serial.print(" | Right: "); Serial.println(results[2]);
        
        _delay_ms(20);
    }
}