#include "wifi_handler.h"
#include "index_html.h"
#include "ble_handler.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.println("WebSocket client connected");
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.println("WebSocket client disconnected");
    }
    else if (type == WS_EVT_DATA)
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
        {
            data[len] = 0;
            String message = String((char *)data);
            Serial.println("Received via WebSocket: " + message);
            sendBLEMessage(message);       // Forward to BLE
            ws.textAll("Web: " + message); // Broadcast to WebSocket clients
        }
    }
}

void setupWiFi()
{
    WiFiManager wifiManager;
    wifiManager.setTimeout(180);
    if (!wifiManager.autoConnect("BLETalk_ESP32"))
    {
        Serial.println("Failed to connect to WiFi, restarting...");
        ESP.restart();
    }
    Serial.println("WiFi connected. IP address: " + WiFi.localIP().toString());

    ws.onEvent(onWebSocketEvent);
    server.addHandler(&ws);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html); });

    server.begin();
    Serial.println("Web server started");
}

void sendWebSocketMessage(const String &message)
{
    ws.textAll(message);
}

void cleanupWebSocketClients()
{
    ws.cleanupClients();
}