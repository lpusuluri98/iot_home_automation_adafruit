# 🏠 Smart Home Automation using ESP8266 & Adafruit IO

This is a simple smart home automation project using an **ESP8266 (NodeMCU)**. It allows you to:

- Control devices (represented using LEDs) from the **Adafruit IO** dashboard.
- Send **temperature and humidity** data from a **DHT11 sensor** to Adafruit IO for real-time monitoring.

---

## 🚀 Features

- 🔌 Control LEDs via Adafruit IO buttons.
- 🌡️ Monitor temperature and humidity.
- 📶 Uses Wi-Fi and MQTT to communicate with Adafruit IO.
- 🔒 Keeps credentials secure via a `secrets.h` file.

---

## 🧰 Hardware Required

- ESP8266 NodeMCU
- DHT11 Temperature & Humidity Sensor
- LEDs
- Breadboard + jumper wires
- Wi-Fi access

---

## 📲 Dashboard

You will use [Adafruit IO](https://io.adafruit.com/) to:

- Create **toggle buttons** to control each LED.
- Display **temperature** and **humidity** values as gauges or text.

---

## 🔧 Setup Instructions

1. Clone/download this repository.
2. Install the following libraries in Arduino IDE:
   - Adafruit MQTT Library
   - DHT sensor library
   - Adafruit Unified Sensor
3. Create a `secrets.h` file (explained below).
4. Upload the code to your ESP8266 using Arduino IDE.
5. Open the Adafruit IO dashboard and test the controls.

---

## 🔐 secrets.h Format

> ⚠️ This file contains sensitive information and should **NOT** be shared publicly. Make sure it's excluded from version control (i.e., add to `.gitignore`).

Create a file named `secrets.h` in the same folder as your `.ino` file:

```cpp
// secrets.h

#define WLAN_SSID       "YourWiFiName"
#define WLAN_PASS       "YourWiFiPassword"

#define AIO_USERNAME    "your_adafruit_io_username"
#define AIO_KEY         "your_adafruit_io_key"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
