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

    while (1)
    {
        Serial.print("Start measurement... ");
        Ultrasonic_TriggerAll();
        Ultrasonic_WaitAll();

        float f = sensor_front.distance;
        float l = sensor_left.distance;
        float r = sensor_right.distance;

        Serial.print("Front: ");
        if (f < 0) Serial.println("no echo");
        else        Serial.println(f, 1);

        Serial.print("Left:  ");
        if (l < 0) Serial.println("no echo");
        else        Serial.println(l, 1);

        Serial.print("Right: ");
        if (r < 0) Serial.println("no echo");
        else        Serial.println(r, 1);

        Serial.println("---");

        delay(200);
    }
}