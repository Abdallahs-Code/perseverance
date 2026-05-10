#include "i2c.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/* =========================================================
   INTERNAL STATE MACHINE
========================================================= */

#define TW_START        0x08
#define TW_REP_START    0x10
#define TW_MT_SLA_ACK   0x18
#define TW_MT_SLA_NACK  0x20
#define TW_MT_DATA_ACK  0x28
#define TW_MT_DATA_NACK 0x30
#define TW_MR_SLA_ACK   0x40
#define TW_MR_SLA_NACK  0x48
#define TW_MR_DATA_ACK  0x50
#define TW_MR_DATA_NACK 0x58

typedef enum
{
    I2C_IDLE = 0,
    I2C_STARTED,
    I2C_REG_SENT,
    I2C_WRITING,
    I2C_REP_STARTED,
    I2C_READING,
    I2C_READ_LAST,
    I2C_DONE,
    I2C_ERROR
} I2C_State;

static volatile I2C_State state = I2C_IDLE;

/* transfer parameters */
static uint8  dev_addr;
static uint8  reg_addr;
static uint8 *tx_ptr;
static uint8  tx_remaining;
static uint8 *rx_ptr;
static uint8  rx_remaining;
static uint8  rx_total;

/* ── debug exports (read from main after IsBusy() clears) ── */
volatile uint8 i2c_last_status = 0;
volatile uint8 i2c_last_state  = 0;

/* =========================================================
   HELPER
========================================================= */

#define TWI_GO(extra)  (TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE)|(extra))

/* =========================================================
   PUBLIC
========================================================= */

void I2C_Init(void)
{
    TWSR = 0x00;
    TWBR = 72;
    TWCR = (1 << TWEN) | (1 << TWIE);
}

void I2C_Transfer(uint8  addr,
                  uint8  reg,
                  uint8 *tx_data,
                  uint8  tx_len,
                  uint8 *rx_buf,
                  uint8  rx_len)
{
    dev_addr     = addr;
    reg_addr     = reg;
    tx_ptr       = tx_data;
    tx_remaining = tx_len;
    rx_ptr       = rx_buf;
    rx_remaining = rx_len;
    rx_total     = rx_len;

    state = I2C_STARTED;
    TWI_GO((1 << TWSTA));
}

uint8 I2C_IsBusy(void)
{
    return (state != I2C_IDLE &&
            state != I2C_DONE &&
            state != I2C_ERROR);
}

/* =========================================================
   TWI INTERRUPT
========================================================= */

ISR(TWI_vect)
{
    uint8 status = (TWSR & 0xF8);

    switch (state)
    {
        case I2C_STARTED:
            if (status == TW_START || status == TW_REP_START)
            {
                TWDR  = (dev_addr << 1);
                TWI_GO(0);
                state = I2C_REG_SENT;
            }
            else { state = I2C_ERROR; TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); }
            break;

        case I2C_REG_SENT:
            if (status == TW_MT_SLA_ACK)
            {
                TWDR  = reg_addr;
                TWI_GO(0);

                if (tx_remaining > 0)
                    state = I2C_WRITING;
                else if (rx_remaining > 0)
                    state = I2C_REP_STARTED;
                else
                    state = I2C_DONE;
            }
            else { state = I2C_ERROR; TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); }
            break;

        case I2C_WRITING:
            if (status == TW_MT_DATA_ACK)
            {
                if (tx_remaining > 0)
                {
                    TWDR = *tx_ptr++;
                    tx_remaining--;
                    TWI_GO(0);
                }
                else if (rx_remaining > 0)
                {
                    TWI_GO((1 << TWSTA));
                    state = I2C_REP_STARTED;
                }
                else
                {
                    TWCR  = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
                    state = I2C_DONE;
                }
            }
            else { state = I2C_ERROR; TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); }
            break;

        case I2C_REP_STARTED:
            if (status == TW_MT_DATA_ACK)
            {
                TWI_GO((1 << TWSTA));
            }
            else if (status == TW_REP_START || status == TW_START)
            {
                TWDR  = (dev_addr << 1) | 1;
                TWI_GO(0);
                state = I2C_READING;
            }
            else { state = I2C_ERROR; TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); }
            break;

        case I2C_READING:
            if (status == TW_MR_SLA_ACK)
            {
                if (rx_remaining > 1)
                    TWI_GO((1 << TWEA));
                else
                    TWI_GO(0);
            }
            else if (status == TW_MR_DATA_ACK)
            {
                *rx_ptr++ = TWDR;
                rx_remaining--;

                if (rx_remaining > 1)
                    TWI_GO((1 << TWEA));
                else
                    TWI_GO(0);
            }
            else if (status == TW_MR_DATA_NACK)
            {
                *rx_ptr = TWDR;
                TWCR  = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
                state = I2C_DONE;
            }
            else { state = I2C_ERROR; TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); }
            break;

        default:
            TWCR  = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
            state = I2C_ERROR;
            break;
    }

    /* ── capture debug snapshot at every ISR exit ── */
    i2c_last_status = status;
    i2c_last_state  = (uint8)state;
}