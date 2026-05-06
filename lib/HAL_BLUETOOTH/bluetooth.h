#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdint.h>

void Bluetooth_Init(uint32_t baudRate);
void Bluetooth_Send(const char* message);

#endif