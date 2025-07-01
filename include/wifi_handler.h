#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

void setupWiFi();
void sendWebSocketMessage(const String &message);
void cleanupWebSocketClients();

#endif