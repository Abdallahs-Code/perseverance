#include "bluetooth.h"

void Bluetooth_Init(long baudRate)
{
  Serial.begin(baudRate);
}

void Bluetooth_Send(String message)
{
  Serial.print("Message Sent From Arduino: ");
  Serial.println(message);
}