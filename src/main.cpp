#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// UUIDs for the BLE service and characteristic
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// WiFi credentials
const char *ssid = "YourWiFiSSID";
const char *password = "YourWiFiPassword";

// Device name for BLE
String deviceName = "BLETalk";
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;

// AsyncWebServer and WebSocket
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Callback for BLE connection/disconnection
class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    Serial.println("BLE Device connected");
  }

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    Serial.println("BLE Device disconnected");
    BLEDevice::startAdvertising(); // Restart advertising
  }
};

// Callback for BLE characteristic write
class MyCharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0)
    {
      String message = String(value.c_str());
      Serial.println("Received via BLE: " + message);

      // Forward to WebSocket clients
      ws.textAll("BLE: " + message);

      // Send response back via BLE
      String response = "received:" + message;
      pCharacteristic->setValue(response.c_str());
      pCharacteristic->notify();
      Serial.println("BLE Response sent: " + response);
    }
  }
};

// WebSocket event handler
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

      // Forward to BLE if connected
      if (deviceConnected)
      {
        pCharacteristic->setValue(message.c_str());
        pCharacteristic->notify();
        Serial.println("Sent via BLE: " + message);
      }

      // Broadcast to all WebSocket clients
      ws.textAll("Web: " + message);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  // Initialize BLE
  uint64_t chipMac = ESP.getEfuseMac();
  uint32_t chipId = (uint32_t)(chipMac & 0xFFFFFFFF);
  deviceName = deviceName + String("_") + String(chipId, HEX);
  BLEDevice::init(deviceName.c_str());
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  BLEDevice::startAdvertising();
  Serial.println("BLE started! Connect to " + deviceName);

  // Initialize WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected. IP address: " + WiFi.localIP().toString());

  // Initialize WebSocket
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  // Serve the chat webpage
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Chat</title>
  <style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    #chatBox { width: 100%; height: 300px; border: 1px solid #ccc; overflow-y: scroll; padding: 10px; }
    #messageInput { width: 80%; padding: 5px; }
    button { padding: 5px 10px; }
  </style>
</head>
<body>
  <h1>ESP32 BLE & Web Chat</h1>
  <div id="chatBox"></div>
  <input type="text" id="messageInput" placeholder="Type a message...">
  <button onclick="sendMessage()">Send</button>
  <script>
    const ws = new WebSocket('ws://' + window.location.hostname + '/ws');
    const chatBox = document.getElementById('chatBox');
    const messageInput = document.getElementById('messageInput');

    ws.onmessage = function(event) {
      const message = document.createElement('div');
      message.textContent = event.data;
      chatBox.appendChild(message);
      chatBox.scrollTop = chatBox.scrollHeight;
    };

    function sendMessage() {
      const message = messageInput.value.trim();
      if (message) {
        ws.send(message);
        messageInput.value = '';
      }
    }

    messageInput.addEventListener('keypress', function(e) {
      if (e.key === 'Enter') {
        sendMessage();
      }
    });
  </script>
</body>
</html>
)rawliteral"); });

  server.begin();
  Serial.println("Web server started");
}

void loop()
{
  ws.cleanupClients();
  delay(10);
}