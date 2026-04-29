#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "../CORE/std_types.h"
#include "../MCAL_GPIO/gpio.h"
#include "../../include/pin_config.h"

#define SOUND_SPEED_CM_US    0.0343f
#define TRIGGER_PULSE_US     10
#define MAX_ECHO_US          30000UL
#define MIN_STARTUP_DELAY_MS 60

#ifdef __cplusplus
extern "C" {
#endif

void ultrasonicBegin(void);
unsigned long ultrasonicReadRawPulse(void);
float ultrasonicReadDistanceCm(void);

#ifdef __cplusplus
}
#endif

#endif