#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "../CORE/std_types.h"
#include "../MCAL_GPIO/gpio.h"
#include "../../include/pin_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ULTRASONIC_NO_ECHO   -1.0f

typedef struct {
    volatile float   distance;    // cm
    volatile uint8   ready;
    volatile uint8   edge_state;
    volatile uint16  rising_time;
} UltrasonicSensor;

extern UltrasonicSensor sensor_front;
extern UltrasonicSensor sensor_left;
extern UltrasonicSensor sensor_right;

// Init GPIO directions and timers
void Ultrasonic_Init(void);

// Fire all three triggers simultaneously, ISRs take over
void Ultrasonic_TriggerAll(void);

// Block caller until all three sensors finish or timeout
void Ultrasonic_WaitAll(void);

#ifdef __cplusplus
}
#endif

#endif /* ULTRASONIC_H */