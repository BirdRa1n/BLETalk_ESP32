# BLETalk_ESP32

## Overview

BLETalk_ESP32 is a project designed to facilitate communication between devices using Bluetooth Low Energy (BLE) and WebSocket over Wi-Fi. This project is built on the ESP32 platform and utilizes the Arduino framework, allowing for easy integration and development of IoT applications.

## Features

- **BLE Communication**: Allows devices to connect and communicate using BLE.
- **WebSocket Support**: Enables real-time communication between web clients and the ESP32.
- **Web Interface**: A simple web interface for users to send and receive messages.
- **Wi-Fi Management**: Automatically connects to Wi-Fi networks using the WiFiManager library.

## Project Structure

```
BLETalk_ESP32/
├── include/
│   ├── ble_handler.h
│   ├── index_html.h
│   └── wifi_handler.h
├── lib/
│   └── README
├── src/
│   ├── ble_handler.cpp
│   ├── main.cpp
│   └── wifi_handler.cpp
├── test/
│   └── README
├── .gitignore
├── LICENSE
├── README.md
└── platformio.ini
```

### Key Files

- **`include/ble_handler.h`**: Header file for BLE functionalities, including setup and message handling.
- **`include/wifi_handler.h`**: Header file for Wi-Fi and WebSocket functionalities.
- **`src/ble_handler.cpp`**: Implementation of BLE functionalities.
- **`src/wifi_handler.cpp`**: Implementation of Wi-Fi and WebSocket functionalities.
- **`src/main.cpp`**: Main entry point of the application, initializing BLE and Wi-Fi.
- **`include/index_html.h`**: Contains the HTML code for the web interface.

## Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/BirdRa1n/BLETalk_ESP32.git
   cd BLETalk_ESP32
   ```

2. **Install PlatformIO**: Ensure you have PlatformIO installed. You can install it as a plugin in Visual Studio Code or use the command line.

3. **Open the Project**: Open the project folder in PlatformIO.

4. **Install Dependencies**: The required libraries will be automatically installed based on the `platformio.ini` configuration.

5. **Upload the Code**: Connect your ESP32 board and upload the code using PlatformIO.

## Usage

1. **Power on the ESP32**: Once the code is uploaded, power on the ESP32.

2. **Connect to Wi-Fi**: The ESP32 will create a Wi-Fi access point named `ESP32_Chat_AP`. Connect to this network.

3. **Access the Web Interface**: Open a web browser and navigate to `http://192.168.4.1` to access the chat interface.

4. **Send Messages**: Type a message in the input field and click "Send" or press "Enter" to send the message. Messages will be sent via BLE and broadcasted to all connected WebSocket clients.

## Code Explanation

### BLE Handler

- **`setupBLE()`**: Initializes the BLE server, creates a service and characteristic, and starts advertising.
- **`sendBLEMessage(const String &message)`**: Sends a message via BLE if a device is connected.

### Wi-Fi Handler

- **`setupWiFi()`**: Sets up the Wi-Fi connection using WiFiManager and initializes the WebSocket server.
- **`onWebSocketEvent()`**: Handles WebSocket events such as connection, disconnection, and incoming messages.
- **`sendWebSocketMessage(const String &message)`**: Broadcasts a message to all connected WebSocket clients.
- **`cleanupWebSocketClients()`**: Cleans up disconnected WebSocket clients.

### Main Application

- **`setup()`**: Initializes the serial communication, Wi-Fi, and BLE.
- **`loop()`**: Continuously cleans up WebSocket clients.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue for any suggestions or improvements.

## Acknowledgments

- [ESP32](https://www.esp32.com/) for the hardware platform.
- [Arduino](https://www.arduino.cc/) for the development framework.
- [PlatformIO](https://platformio.org/) for the project management and build system.
- [WiFiManager](https://github.com/tzapu/WiFiManager) for managing Wi-Fi connections.
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) for handling WebSocket connections.