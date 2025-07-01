#ifndef BLE_HANDLER_H
#define BLE_HANDLER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

extern BLECharacteristic *pCharacteristic;
extern bool deviceConnected;

void setupBLE();
void sendBLEMessage(const String &message);

#endif