#include "bluetooth.h"
#include <avr/io.h>

void Bluetooth_Init(uint32_t baudRate)
{
  // calculates the value to put into the UART baud rate register
  uint16_t ubrr = (F_CPU / (16UL * baudRate)) - 1;

  // UART BAUD RATE REIGISTER
  UBRR1H = (uint8_t)(ubrr >> 8);
  UBRR1L = (uint8_t)(ubrr);

  // USART Control and Status Register 0 B
  // ENABLE TX1 and RX1
  UCSR1B = (1 << TXEN1) | (1 << RXEN1);

  // USART Control and Status Register 0 C
  // 8 bits data one stop bit
  // UCSZ12  UCSZ11  UCSZ10    Data Bits
  // 0       0       0         5 bits
  // 0       0       1         6 bits
  // 0       1       0         7 bits
  // 0       1       1         8 bits ← most common
  // 1       1       1         9 bits
  // USBS1 = 0 meaning stop bit = 0
  UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);

}

void Bluetooth_Send(const char *message)
{
  const char *prefix = "Message Sent From Arduino: ";
  while (*prefix)
  {
    // UCSRnA : This is the USART Control and Status Register A
    // status flags like:
    // - Is transmit buffer empty?
    // - Is receive complete?
    // - Errors, etc.
    // UDREn : USART Data Register Empty
    // This bit = 1 when The transmit buffer is ready to accept new data
    // UCSRnA & (1<<UDREn) This checks: Is UDRE bit = 1?
    while (!(UCSR1A & (1 << UDRE1)));
    
    // put data into buffer
    UDR1 = *prefix++;
    
  }

  while (*message)
  {
    while (!(UCSR1A & (1 << UDRE1)));
    UDR1 = *message++;
  }
}