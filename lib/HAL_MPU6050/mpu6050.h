#ifndef MPU6050_H
#define MPU6050_H

#include "std_types.h"

/* =========================================================
   MPU6050 — interrupt-driven Z-axis gyro driver

   Flow:
     MPU6050_Init()           — once at startup
     MPU6050_SetBias(bias)    — after calibration in main
     MPU6050_RequestRead()    — called from Timer ISR
     MPU6050_ProcessResult()  — called from main loop when ready
     MPU6050_GetAngleZ()      — read accumulated angle anytime
========================================================= */

void  MPU6050_Init          (void);
void  MPU6050_SetBias       (float bias);

/* Kick off a non-blocking gyro Z read via I2C */
void  MPU6050_RequestRead   (void);

/* Returns 1 when a fresh reading is ready to process */
uint8 MPU6050_ResultReady   (void);

/*
 * MPU6050_ProcessResult
 * ---------------------
 * Call from main loop after ResultReady() returns 1.
 * Converts raw bytes, applies bias + dead-zone,
 * integrates with dt into angleZ.
 * Clears the ready flag.
 */
void  MPU6050_ProcessResult (float dt);

float MPU6050_ReadGyroZ     (void);   /* blocking — for calibration only */
void  MPU6050_ResetAngleZ   (void);
float MPU6050_GetAngleZ     (void);
uint8 MPU6050_WhoAmI        (void);   /* blocking — for debug only */

#endif /* MPU6050_H */