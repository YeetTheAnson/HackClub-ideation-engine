# HackClub-ideation-engine
A esp32 code that randomly picks a project idea, and has a one hour timer when you start a session.


https://github.com/user-attachments/assets/5227019f-28d5-473a-9462-d9f0aae26bf7


## Getting Started

### Prerequisites

Before you begin, ensure you have the following libraries installed in your Arduino IDE:

- `Adafruit_GFX.h`
- `Adafruit_ILI9341.h`
- `WiFi.h`
- `HTTPClient.h`

### Configuration

To configure the ESP32 to connect to your Wi-Fi network, you need to modify the default Wi-Fi credentials in the code. Open the source file and locate the following lines:

```
#define DEFAULTSSID "your_wifi_ssid"
#define DEFAULTPASS "your_wifi_password"
```

### Circuit Diagram
The connections between the ESP32 and the ILI9341 display, as well as the button setup.


| ESP32 Pin | ILI9341 Pin |
|-----------|-------------|
| 5V        | VCC         |
| GND       | GND         |
| 15        | CS          |
| 4         | RST         |
| 2         | D/C         |
| 23        | MOSI        |
| 18        | SCK         |
| 19        | MISO        |

#### Button Connections

| ESP32 Pin | Generate Button | Start Timer Button |
|-----------|------------------|---------------------|
| 5         | ✅                | -                   |
| 22        | -                | ✅                   |


![alt text](https://raw.githubusercontent.com/YeetTheAnson/HackClub-ideation-engine/main/WokWiCircuit.png)
