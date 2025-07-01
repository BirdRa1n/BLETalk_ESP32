#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// UUIDs for the service and characteristic
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

String deviceName = "BLETalk";
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;

// Callback for connection/disconnection
class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    Serial.println("Device connected");
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    Serial.println("Device disconnected");
    BLEDevice::startAdvertising(); // Restart advertising
  }
};

// Callback for writing to the characteristic
class MyCharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0)
    {
      Serial.print("Received via BLE: ");
      for (int i = 0; i < value.length(); i++)
      {
        Serial.print(value[i]);
      }
      Serial.println();

      // Send a response (echo of the received text)
      String response = "received:" + String(value.c_str());
      pCharacteristic->setValue(response.c_str());
      pCharacteristic->notify(); // Notify the client with the response
      Serial.println("Response sent: " + response);
    }
  }
};

void setup()
{
  // Initialize the serial monitor
  Serial.begin(115200);

  // Set the device name
  uint64_t chipMac = ESP.getEfuseMac();
  uint32_t chipId = (uint32_t)(chipMac & 0xFFFFFFFF);
  deviceName = deviceName + String("_") + String(chipId, HEX);
  // Initialize the BLE device
  BLEDevice::init(deviceName.c_str());
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create the BLE characteristic with read, write and notify properties
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY);

  // Associate the callback with the characteristic
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  // Add descriptor to support notifications
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // Improve compatibility with iOS
  BLEDevice::startAdvertising();
  Serial.println("BLE started! Connect to " + deviceName);
}

void loop()
{
  if (deviceConnected)
  {
    // Send a message typed via the serial monitor
    if (Serial.available())
    {
      String input = Serial.readStringUntil('\n');
      input.trim(); // remove \r\n or extra spaces
      if (input.length() > 0)
      {
        pCharacteristic->setValue(input.c_str());
        pCharacteristic->notify();
        Serial.println("Sent via BLE: " + input);
      }
    }
  }

  delay(10);
}
