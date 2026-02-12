# Security Audit Report - Home Electrical Measurement Project

## 🔒 Security Assessment

This document provides a security audit of the HomeElectricalMeasurement project, identifying sensitive information that should be protected and providing recommendations for securing the deployment.

---

## ⚠️ CRITICAL FINDINGS

### 1. Hardcoded Credentials in Source Code

The following files contain hardcoded credentials that **MUST BE CHANGED** before deployment:

#### All Files
- **Location**: Multiple .ino files
- **Issue**: Mesh network credentials hardcoded
  ```cpp
  #define MESH_PREFIX     "whateverYouLike"
  #define MESH_PASSWORD   "somethingSneaky"
  ```
- **Risk**: Low security passwords using default example values
- **Affected Files**:
  - HEM_Server_v0.1/HEM_Server_v0.1.ino
  - HEM_Server_v0.2/HEM_Server_v0.2.ino
  - HEM_Server_v0.3/HEM_Server_v0.3.ino
  - HEM_Client_v0.1/HEM_Client_v0.1.ino
  - HEM_Client_v0.2/HEM_Client_v0.2.ino

#### HEM_Server_v0.3 (Most Critical)

**WiFi Credentials:**
- **Line 17-18**:
  ```cpp
  #define STATION_SSID     "Freebox_BSHOQE"
  #define STATION_PASSWORD "?"
  ```
- **Risk**: Reveals WiFi network name; password placeholder needs to be filled
- **Impact**: Anyone with access to the code knows the WiFi network name

**MQTT Credentials:**
- **Line 20-21**:
  ```cpp
  #define MQTT_USER     "admin"
  #define MQTT_PASSWORD "?"
  ```
- **Risk**: Default "admin" username; password placeholder
- **Impact**: If MQTT broker is exposed, weak credentials could allow unauthorized access

**Hardcoded in Connection Logic:**
- **Line 190**:
  ```cpp
  if (mqttClient.connect("arduinoClient", "admin", "14258")) {
  ```
- **Line 282**:
  ```cpp
  if (mqttClient.connect("painlessMeshClient","admin","147258")) {
  ```
- **Risk**: Multiple different MQTT passwords hardcoded in the code
- **Impact**: Passwords visible in plain text in source code

**IP Addresses:**
- **Line 56**:
  ```cpp
  IPAddress mqttBroker(192, 168, 1, 42);
  ```
- **Risk**: Reveals internal network topology
- **Impact**: Low - local network IP, but still information disclosure

---

## 🛡️ RECOMMENDATIONS

### Immediate Actions Required

1. **Create Configuration Files**
   
   Create a `config.h` file in each project directory (DO NOT commit these files):

   ```cpp
   // config.h - NEVER COMMIT THIS FILE
   #ifndef CONFIG_H
   #define CONFIG_H
   
   // Mesh Network - Use strong passwords
   #define MESH_PREFIX     "YourUniqueMeshName"
   #define MESH_PASSWORD   "YourSecurePassword123!"
   #define MESH_PORT       5555
   
   // WiFi Station (for Server v0.3)
   #define STATION_SSID     "YourWiFiNetwork"
   #define STATION_PASSWORD "YourWiFiPassword"
   
   // MQTT Configuration (for Server v0.3)
   #define MQTT_BROKER_IP_1   192
   #define MQTT_BROKER_IP_2   168
   #define MQTT_BROKER_IP_3   1
   #define MQTT_BROKER_IP_4   42
   #define MQTT_USER          "your_mqtt_username"
   #define MQTT_PASSWORD      "YourSecureMQTTPassword123!"
   #define MQTT_PORT          1883
   
   #endif
   ```

2. **Update .ino Files**
   
   Modify each .ino file to include the config file:
   ```cpp
   #include "config.h"
   ```
   
   And remove the hardcoded credential definitions.

3. **Use Strong Passwords**
   
   - Minimum 12 characters
   - Mix of uppercase, lowercase, numbers, and special characters
   - Avoid common words or patterns
   - Use different passwords for each service

4. **Protect Configuration Files**
   
   - The `.gitignore` file is configured to exclude `config.h` files
   - Verify configuration files are not tracked in git:
     ```bash
     git status
     ```
   - If accidentally committed, remove from git history

### Network Security

1. **Mesh Network**
   - Change default MESH_PREFIX and MESH_PASSWORD
   - Use WPA2 encryption level passwords
   - Consider using unique mesh names for each deployment

2. **WiFi Network**
   - Use WPA3 or WPA2 on your WiFi router
   - Ensure strong WiFi password
   - Consider using a separate IoT network/VLAN

3. **MQTT Broker**
   - Change default MQTT username from "admin"
   - Use strong MQTT passwords
   - Enable authentication on MQTT broker
   - Consider using MQTT over TLS (MQTTS)
   - Restrict MQTT broker to local network only
   - Use ACL (Access Control Lists) on MQTT broker

### Code Security

1. **Remove Commented Credentials**
   - Review all commented code for credentials
   - Clean up before committing

2. **Avoid Multiple Password Definitions**
   - Use consistent credential references
   - Single source of truth for passwords

3. **Regular Updates**
   - Keep ESP8266 core updated
   - Update libraries regularly for security patches

---

## 📋 SECURITY CHECKLIST

Before deploying this project, ensure:

- [ ] Changed all default passwords (mesh, WiFi, MQTT)
- [ ] Created `config.h` files with secure credentials
- [ ] Verified `config.h` is in `.gitignore`
- [ ] Removed or replaced example credentials in code
- [ ] Used strong, unique passwords for each service
- [ ] MQTT broker is not exposed to internet (or uses TLS if it is)
- [ ] WiFi network uses WPA2/WPA3 encryption
- [ ] Reviewed all code for hardcoded credentials
- [ ] Tested that devices connect successfully with new credentials

---

## 🔍 FILES REQUIRING ATTENTION

| File | Line(s) | Issue | Priority |
|------|---------|-------|----------|
| HEM_Server_v0.3.ino | 13, 17-18, 20-21 | WiFi & MQTT credentials | **CRITICAL** |
| HEM_Server_v0.3.ino | 56 | Hardcoded MQTT broker IP | Medium |
| HEM_Server_v0.3.ino | 190, 282 | Hardcoded MQTT passwords in logic | **CRITICAL** |
| HEM_Server_v0.2.ino | 10 | Mesh password | High |
| HEM_Server_v0.1.ino | 39 | Mesh password | High |
| HEM_Client_v0.2.ino | 9 | Mesh password | High |
| HEM_Client_v0.1.ino | 12 | Mesh password | High |

---

## 📚 Additional Resources

- [ESP8266 Security Best Practices](https://arduino-esp8266.readthedocs.io/en/latest/)
- [MQTT Security Fundamentals](https://www.hivemq.com/mqtt-security-fundamentals/)
- [Arduino Security Guidelines](https://www.arduino.cc/en/security)
- [OWASP IoT Security](https://owasp.org/www-project-internet-of-things/)

---

## 📝 Notes

- This is example/educational code - credentials shown are placeholders
- Some credentials use "?" as placeholders indicating they need to be filled
- The project is for personal use on a local network
- No credentials in this project are intended for production use
- All sensitive data should be stored in configuration files excluded from version control

---

**Last Updated**: 2026-02-12  
**Auditor**: Security Review - Automated Scan  
**Status**: Initial Security Assessment Complete
