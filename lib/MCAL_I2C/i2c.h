#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "std_types.h"

void  I2C_Init(void);
void  I2C_Transfer(uint8 addr, uint8 reg,
                   uint8 *tx_data, uint8 tx_len,
                   uint8 *rx_buf,  uint8 rx_len);
uint8 I2C_IsBusy(void);

/* debug: last TWI status byte and internal state enum value */
extern volatile uint8 i2c_last_status;
extern volatile uint8 i2c_last_state;

#endif /* I2C_H */