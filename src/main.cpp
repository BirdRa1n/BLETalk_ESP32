#include <Arduino.h>
#include "ble_handler.h"
#include "wifi_handler.h"

void setup()
{
  Serial.begin(115200);
  setupWiFi(); // Initialize WiFi and WebSocket server
  setupBLE();  // Initialize BLE server
  Serial.println("System initialized");
}

void loop()
{
  cleanupWebSocketClients(); // Clean up WebSocket clients
  delay(10);
}