#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

void Bluetooth_Init(long baudRate);
void Bluetooth_Send(String message);

#endif