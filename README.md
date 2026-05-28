# Smart AgroCare System using Arduino

An Arduino-based smart farming system that automates plant monitoring and environmental control using sensors and actuators.

---

## Features

- Multi-plant support
- Growth-stage based monitoring
- Automatic irrigation control
- Light intensity monitoring
- Temperature & humidity monitoring
- Smoke/fire detection
- LCD user interface
- Keypad-based plant selection
- Buzzer alerts and notifications

---

## Supported Plants

| Key | Plant |
|-----|------|
| 1 | No Plant |
| 2 | Tomato |
| 3 | Cucumber |
| 4 | Rose |
| 5 | Spinach |
| 6 | Strawberry |

---

## Hardware Components

- Arduino UNO
- Arduino Sensor Kit
- 16x2 I2C LCD Display
- 4x3 Keypad
- Soil Moisture Sensor
- DHT Temperature & Humidity Sensor
- Photoresistor / LDR
- Smoke/Gas Sensor
- Buzzer
- DC Water Pump
- Motor Driver
- LED Grow Light
- Jumper Wires
- Breadboard

---

## Working Principle

1. User selects a plant using the keypad.
2. User enters plant height.
3. System determines growth stage.
4. Sensors continuously monitor:
   - Soil moisture
   - Temperature
   - Humidity
   - Light intensity
   - Smoke level
5. Based on threshold values:
   - Water pump activates
   - LED grow light turns ON/OFF
   - Alerts are shown on LCD
   - Buzzer notifications are generated

---

## Sensors Used

### Soil Moisture Sensor
Measures soil water content and controls irrigation.

### Temperature & Humidity Sensor
Monitors environmental conditions suitable for plant growth.

### Light Sensor (LDR)
Measures light intensity and controls artificial lighting.

### Smoke/Gas Sensor
Detects smoke/fire hazards for safety.

---

## Software Requirements

- Arduino IDE
- Required Libraries:
  - Keypad.h
  - Arduino_SensorKit.h
  - LiquidCrystal_I2C.h
  - Wire.h

---

## Pin Connections

| Component | Pin |
|-----------|-----|
| Keypad Rows | 2,3,4,5 |
| Keypad Columns | 6,10,7 |
| Buzzer | 8 |
| Motor ENA | 11 |
| Motor IN1 | A0 |
| Motor IN2 | 9 |
| Soil Moisture Sensor | A1 |
| Smoke Sensor | A3 |
| LED Grow Light | A2 |
| LCD I2C | SDA/SCL |

---

## Functionalities

### Automatic Irrigation
The pump activates automatically when soil moisture falls below plant-specific thresholds.

### Intelligent Lighting
LED grow light turns ON/OFF depending on ambient light intensity.

### Environmental Monitoring
Displays:
- Temperature
- Humidity
- Moisture percentage
- Light percentage

### Fire Detection
Triggers buzzer and warning alerts when smoke is detected.

---

## Future Improvements

- IoT integration using ESP8266/ESP32
- Mobile app monitoring
- Cloud data logging
- Automatic weather prediction
- AI-based crop recommendations
- Solar-powered system

---

## Applications

- Smart Farming
- Greenhouses
- Home Gardening
- Hydroponics
- Agricultural Research

---

## Team

Circuit Breakers

---

## License

This project is open-source and available under the MIT License.
