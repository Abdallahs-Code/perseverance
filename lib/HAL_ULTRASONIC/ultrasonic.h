#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "../CORE/std_types.h"
#include "../MCAL_GPIO/gpio.h"
#include "../../include/pin_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ULTRASONIC_NO_ECHO -1.0f

#define ULTRASONIC_THRESHOLD_FRONT 20.0f   
#define ULTRASONIC_THRESHOLD_LEFT 20.0f   
#define ULTRASONIC_THRESHOLD_RIGHT 20.0f   

#define ULTRASONIC_BELOW_THRESHOLD 1
#define ULTRASONIC_ABOVE_THRESHOLD 0

typedef struct {
    volatile float   distance;    // cm
    volatile uint8   ready;
    volatile uint8   edge_state;
    volatile uint16  rising_time;
} UltrasonicSensor;

UltrasonicSensor sensor_front;
UltrasonicSensor sensor_left;
UltrasonicSensor sensor_right;

// Init GPIO directions and timers
void Ultrasonic_Init(void);

void Ultrasonic_CheckSensors(float distances[3], uint8 results[3]);

#ifdef __cplusplus
}
#endif

#endif