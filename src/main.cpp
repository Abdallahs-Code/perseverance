#include "../lib/HAL_ULTRASONIC/ultrasonic.h"

int main(void)
{
    ultrasonicBegin();

    while(1)
    {
        float distance = ultrasonicReadDistanceCm();

        if (distance < 0)
        {
            /* Timeout — no object detected */
        }
        else
        {
            /* Distance is in: distance (cm) */
        }

        for(volatile long i = 0; i < 50000; i++);
    }

    return 0;
}