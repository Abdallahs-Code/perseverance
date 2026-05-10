#include "mpu6050.h"
#include "i2c.h"
#include <avr/io.h>

/* =========================================================
   DEFINES
========================================================= */

#define MPU6050_ADDR      0x68
#define PWR_MGMT_1        0x6B
#define GYRO_ZOUT_H       0x47
#define GYRO_CONFIG       0x1B
#define INT_PIN_CFG       0x37
#define INT_ENABLE        0x38
#define GYRO_SENSITIVITY  131.0f
#define MPU_WHO_AM_I      0x75
#define GYRO_DEADZONE     0.15f

/* =========================================================
   STATE
========================================================= */

static float          angleZ     = 0.0f;
static float          bias_val   = 0.0f;
static volatile uint8 result_rdy = 0;
static uint8          gyro_buf[2];

/* =========================================================
   PRIVATE
========================================================= */

static void MPU6050_WriteReg(uint8 reg, uint8 data)
{
    uint8 tx[1] = { data };
    I2C_Transfer(MPU6050_ADDR, reg, tx, 1, 0, 0);
    while (I2C_IsBusy());
}

/* =========================================================
   PUBLIC
========================================================= */

void MPU6050_Init(void)
{
    MPU6050_WriteReg(PWR_MGMT_1,  0x00);
    MPU6050_WriteReg(GYRO_CONFIG, 0x00);
    MPU6050_WriteReg(INT_PIN_CFG, 0x10);  /* active low, 50us pulse */
    MPU6050_WriteReg(INT_ENABLE,  0x01);  /* fire INT on data ready */
}

void MPU6050_SetBias(float bias)
{
    bias_val = bias;
}

void MPU6050_RequestRead(void)
{
    if (I2C_IsBusy()) return;
    result_rdy = 0;
    I2C_Transfer(MPU6050_ADDR, GYRO_ZOUT_H, 0, 0, gyro_buf, 2);
}

/* simply check if I2C finished — no buffer value guard */
uint8 MPU6050_ResultReady(void)
{
    if (!result_rdy && !I2C_IsBusy() && (gyro_buf[0] || gyro_buf[1]))
    {
        result_rdy = 1;
    }
    return result_rdy;
}

void MPU6050_ProcessResult(float dt)
{
    int16 raw;
    float gz;

    raw        = (int16)((gyro_buf[0] << 8) | gyro_buf[1]);
    gz         = ((float)raw / GYRO_SENSITIVITY) - bias_val;

    if (gz > -GYRO_DEADZONE && gz < GYRO_DEADZONE)
        gz = 0.0f;

    angleZ    += gz * dt;
    result_rdy = 0;
}

float MPU6050_ReadGyroZ(void)
{
    uint8 buf[2] = {0, 0};
    int16 raw;

    I2C_Transfer(MPU6050_ADDR, GYRO_ZOUT_H, 0, 0, buf, 2);
    while (I2C_IsBusy());

    raw = (int16)((buf[0] << 8) | buf[1]);
    return (float)raw / GYRO_SENSITIVITY;
}

void MPU6050_ResetAngleZ(void)
{
    angleZ = 0.0f;
}

float MPU6050_GetAngleZ(void)
{
    return angleZ;
}

uint8 MPU6050_WhoAmI(void)
{
    uint8 id = 0;
    I2C_Transfer(MPU6050_ADDR, MPU_WHO_AM_I, 0, 0, &id, 1);
    while (I2C_IsBusy());
    return id;
}