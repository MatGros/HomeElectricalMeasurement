# Home Electrical Measurement (HEM)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## 📋 Description

HEM (Home Electrical Measurement) is a personal IoT project for monitoring home electrical consumption in real-time using ESP8266 microcontrollers. The system detects LED blinks from electrical meters (such as French Linky meters) using light-dependent resistors (LDR) and transmits consumption data over a wireless mesh network with MQTT integration for data visualization and analysis.

## 🎯 Features

- **Real-time electrical consumption monitoring** via LDR sensor detecting meter blinks
- **Wireless mesh network** communication between ESP8266 nodes using painlessMesh
- **MQTT integration** for data publishing and integration with home automation systems
- **Multi-version evolution** showing project development from v0.1 to v0.3
- **Instant power calculation** in VA (Volt-Ampere)
- **Consumption tracking** per 15 minutes and per hour
- **Client-Server architecture** with multiple nodes support

## 🛠️ Hardware Requirements

### ESP8266 Development Boards
- **NodeMCU v3** or similar ESP8266-based boards
- **Witty Cloud** module (for some versions with RGB LED support)

### Components
- **LDR (Light Dependent Resistor)** - Connected to A0 (analog pin)
- **LEDs** - RGB LEDs (pins 15/RED, 12/GREEN, 13/BLUE on Witty Cloud)
- **Button** - Optional (pin 4)
- **USB cable** for programming and power
- **Electrical meter with LED indicator** (e.g., Linky meter in France)

### Pin Configuration (Witty Cloud)
```
LDR:   A0 (Analog pin)
RED:   GPIO 15
GREEN: GPIO 12
BLUE:  GPIO 13
BUTTON: GPIO 4
```

## 📦 Software Dependencies

### Arduino IDE or PlatformIO
- **Arduino IDE** 1.8.x or higher, OR
- **PlatformIO** IDE (VS Code extension)

### Required Libraries
- **painlessMesh** - Mesh network communication library
- **ArduinoJson** (v5 or v6) - JSON parsing and serialization
- **PubSubClient** - MQTT client library (for server versions with MQTT)
- **ESP8266WiFi** - WiFi connectivity (included in ESP8266 core)

### ESP8266 Board Support
- **ESP8266 Core** for Arduino (install via Board Manager)

## 🚀 Installation

### 1. Setup Arduino IDE / PlatformIO

**For Arduino IDE:**
1. Install Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Add ESP8266 board support:
   - Go to File → Preferences
   - Add to "Additional Board Manager URLs": `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   - Go to Tools → Board → Board Manager
   - Search for "ESP8266" and install "ESP8266 by ESP8266 Community"

**For PlatformIO:**
1. Install VS Code and PlatformIO extension
2. The ESP8266 platform will be installed automatically with the project

### 2. Install Required Libraries

**Arduino IDE:**
- Go to Sketch → Include Library → Manage Libraries
- Search and install:
  - painlessMesh
  - ArduinoJson
  - PubSubClient

**PlatformIO:**
Add to `platformio.ini`:
```ini
[env:esp12e]
platform = espressif8266
board = esp12e
framework = arduino
lib_deps = 
    painlessmesh/painlessMesh
    bblanchon/ArduinoJson
    knolleary/PubSubClient
```

### 3. Configure Credentials

⚠️ **IMPORTANT SECURITY NOTE**: This project contains example code with placeholder credentials. Before deploying:

1. **Create a configuration file** (e.g., `config.h`) in each project folder:

```cpp
// config.h - DO NOT COMMIT THIS FILE
#ifndef CONFIG_H
#define CONFIG_H

// Mesh Network Configuration
#define MESH_PREFIX     "YourMeshName"
#define MESH_PASSWORD   "YourSecurePassword"
#define MESH_PORT       5555

// WiFi Station Configuration (for Server with MQTT)
#define STATION_SSID     "YourWiFiSSID"
#define STATION_PASSWORD "YourWiFiPassword"

// MQTT Configuration
#define MQTT_BROKER_IP   "192.168.1.xxx"  // Your MQTT broker IP
#define MQTT_USER        "your_mqtt_user"
#define MQTT_PASSWORD    "your_mqtt_password"

#endif
```

2. **Update the .ino files** to include your config file:
```cpp
#include "config.h"
```

3. **Never commit** your `config.h` file (it's listed in `.gitignore`)

### 4. Upload to ESP8266

1. Connect your ESP8266 board via USB
2. Select the correct board and port in Arduino IDE / PlatformIO
3. Choose the appropriate sketch for your node type:
   - **HEM_Server_v0.3** - Main server node with MQTT bridge
   - **HEM_Client_v0.2** - Client node for additional monitoring points
   - **HEM_MACFinder** - Utility to find your ESP8266 MAC address
   - **Test_LDR** - Test program for LDR sensor calibration
4. Upload the sketch

## 📂 Project Structure

```
HomeElectricalMeasurement/
├── HEM_Server_v0.1/        # Initial server version (basic mesh)
├── HEM_Server_v0.2/        # Server v0.2 with improvements
├── HEM_Server_v0.3/        # Latest server with MQTT integration
├── HEM_Client_v0.1/        # Initial client version
├── HEM_Client_v0.2/        # Improved client version
├── HEM_BlinkSimulator/     # LED blink simulator for testing
├── HEM_MACFinder/          # Utility to find ESP MAC address
├── Test_LDR/               # LDR sensor test program
├── README.md               # This file
├── LICENSE                 # MIT License
└── .gitignore              # Git ignore patterns
```

### Component Descriptions

#### Server Nodes (HEM_Server_v0.x)
- Main node that collects data from LDR sensor
- Calculates instant power consumption (VA)
- Tracks consumption per 15 minutes and per hour
- Broadcasts data over mesh network
- **v0.3**: Acts as MQTT bridge to WiFi network

#### Client Nodes (HEM_Client_v0.x)
- Secondary nodes that receive data from server
- Display consumption information
- Can be placed in different rooms for monitoring
- Communicate via mesh network

#### Utilities
- **HEM_BlinkSimulator**: Simulates electrical meter LED blinks for testing
- **HEM_MACFinder**: Finds the MAC address of your ESP8266
- **Test_LDR**: Calibrates and tests the LDR sensor

## 🔧 Usage

### Basic Setup

1. **Position the LDR sensor** on your electrical meter's LED indicator
   - Ensure good contact and light seal
   - The LDR should clearly detect the blinks

2. **Power on the Server node** (HEM_Server)
   - It will create the mesh network
   - Start monitoring the LDR
   - If using v0.3, it connects to WiFi and MQTT broker

3. **Power on Client nodes** (optional)
   - They will automatically join the mesh network
   - Start receiving consumption data

4. **Monitor via Serial** (115200 baud)
   - Open Serial Monitor in Arduino IDE
   - View real-time consumption data

### MQTT Topics (v0.3)

The server publishes to the following MQTT topics:
```
linky/ServerAlive      # Server heartbeat (0/1)
linky/LDR              # Raw LDR value
linky/ConsoInstVA      # Instantaneous consumption in VA
linky/maxConsoInstVA   # Maximum consumption recorded
linky/WhPer15Minute    # Wh consumed in last 15 minutes
linky/WhPerHour        # Wh consumed in last hour
```

### Interpreting the Data

- **ConsoInstVA**: Current power consumption calculated from blink frequency
  - Formula: `3600 / (time_between_blinks_in_seconds)`
  - Typical values: 100-3000 VA
- **WhPer15Minute**: Number of Wh consumed in 15-minute period
- **WhPerHour**: Total Wh consumed per hour
- **maxConsoInstVA**: Peak consumption recorded

## 🔒 Security Considerations

⚠️ **WARNING**: The example code contains placeholder credentials for demonstration purposes.

### Sensitive Information to Configure

Before deploying this project, you MUST replace the following:

1. **Mesh Network Credentials**
   - `MESH_PREFIX`: Change from default "whateverYouLike"
   - `MESH_PASSWORD`: Change from default "somethingSneaky"

2. **WiFi Credentials** (HEM_Server_v0.3)
   - `STATION_SSID`: Your WiFi network name
   - `STATION_PASSWORD`: Your WiFi password

3. **MQTT Credentials** (HEM_Server_v0.3)
   - `MQTT_USER`: Your MQTT broker username
   - `MQTT_PASSWORD`: Your MQTT broker password
   - `mqttBroker` IP address: Your MQTT broker IP

### Best Practices

- ✅ Create a separate `config.h` file with your credentials (see Installation section)
- ✅ Add `config.h` to `.gitignore` to prevent committing credentials
- ✅ Use strong passwords for mesh and MQTT
- ✅ Change default MQTT credentials
- ✅ Use a secure WiFi network
- ✅ Consider using MQTT over TLS if exposing to internet

## 🤝 Contributing

This is a personal project, but suggestions and improvements are welcome!

If you'd like to contribute:
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

### MIT License Summary

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED.

## 🙏 Acknowledgments

- **painlessMesh** library for easy ESP8266 mesh networking
- **ArduinoJson** for efficient JSON parsing
- **PubSubClient** for MQTT functionality
- ESP8266 community for excellent documentation and support

## 📧 Contact

This is a personal project by MatGros.

For questions or issues, please open an issue on the GitHub repository.

## 🔮 Future Improvements

Potential enhancements for this project:
- [ ] Web interface for real-time monitoring
- [ ] Data logging to SD card or cloud storage
- [ ] Mobile app integration
- [ ] Support for multiple electrical meters
- [ ] Historical data analysis and graphs
- [ ] Alert system for high consumption
- [ ] Integration with Home Assistant or other home automation platforms
- [ ] Over-the-air (OTA) firmware updates
- [ ] Encrypted mesh communication
- [ ] Battery backup support

---

**⚡ Happy Monitoring! ⚡**
