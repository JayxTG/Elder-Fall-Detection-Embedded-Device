# Fall Detection System: Embedded Biomedical Device

This repository contains the implementation of a fall detection system using various sensors and microcontrollers, designed to assist in monitoring and alerting for falls, particularly for the elderly. The project is part of the BM2210: Biomedical Device Design course.

## 🎯 Objective

The objective of this project is to design, implement, and verify a fall detection system that accurately detects falls and alerts caregivers in real-time.

## 🔑 Key Features

- ⚙️ Accurate fall detection using accelerometers and gyroscopes
- 📡 Real-time alert system
- 🌐 Wireless communication for alerts
- 📊 Data logging for fall events

## 🛠️ Hardware and Software Requirements

### Hardware:
- Microcontroller (ESP32)
- Accelerometer and gyroscope sensors (e.g., MPU6050)
- Battery pack
- Buzzer or LED for local alerts

### Software:
- Arduino IDE
- MQTT broker (e.g., test.mosquitto.org) for real-time alerts
- Data visualization tools


## 🎛️ Functionality

1. **Fall Detection Algorithm**
   - Utilize accelerometer and gyroscope data to detect falls
   - Implement threshold-based and machine learning algorithms for fall detection
   - Minimize false positives and false negatives

2. **Alert System**
   - Immediate local alert using buzzer or LED
   - Real-time remote alert using MQTT to notify caregivers

3. **Data Logging**
   - Log fall events with timestamp
   - Optional: Store accelerometer and gyroscope data for further analysis

## 📊 Data Visualization

- Real-time monitoring dashboard 
- Historical data analysis

## 🔗 System Architecture

The system uses MQTT for communication between the fall detection device and the alert system. The MQTT broker used is test.mosquitto.org.

## 📂 Implementation Steps

1. Set up the microcontroller development environment
2. Interface accelerometer and gyroscope sensors with the microcontroller
3. Implement the fall detection algorithm
4. Program the alert system for both local and remote alerts
5. Set up MQTT communication for real-time alerts
6. Test and verify all functionalities with real-world scenarios

## 📚 References

- BM2210: Biomedical Device Design course materials
- Accelerometer and gyroscope sensor datasheets
- MQTT protocol specification

---

This project provides a comprehensive fall detection system with real-time alert capabilities, enhancing the safety and monitoring of individuals at risk of falls.
