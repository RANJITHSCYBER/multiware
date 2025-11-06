# 🤖 ESP32 MultiWare System

A modular ESP32-based platform integrating multiple sensors and peripherals with an OLED menu interface. Ideal for robotics, automation, and embedded experimentation.

---

## 📦 Features

- OLED menu interface with 2x2 grid navigation  
- Button-based UI (Up, Down, Left, Right, Select)  
- Ultrasonic obstacle detection  
- IR sensor input  
- MPU6050 motion sensing  
- Motor and servo control  
- SD card file management  

---

## 🛠️ Hardware Components

- ESP32 Dev Board  
- SH1106 OLED Display (I2C)  
- HC-SR04 Ultrasonic Sensor  
- IR Transmitter and Receiver  
- MPU6050 Accelerometer/Gyroscope  
- L298N Motor Driver  
- Servo Motor  
- SD Card Module (SPI)  
- 5x Push Buttons  
- LED (optional)  

---

## 📐 Pin Configuration

| Component         | Function        | GPIO Pin |
|------------------|------------------|----------|
| Buttons           | UP               | 32       |
|                   | DOWN             | 33       |
|                   | LEFT             | 25       |
|                   | RIGHT            | 26       |
|                   | SELECT           | 27       |
| OLED Display      | SDA              | 21       |
|                   | SCL              | 22       |
| MPU6050           | SDA              | 21       |
|                   | SCL              | 22       |
| IR Sensors        | TX               | 17       |
|                   | RX               | 16       |
| Ultrasonic Sensor | TRIG             | 4        |
|                   | ECHO             | 2        |
| LED               | Status Indicator | 5        |
| SD Card Module    | MOSI             | 23       |
|                   | MISO             | 19       |
|                   | SCK              | 18       |
|                   | CS               | 15       |
| Motor Driver      | ENA              | 13       |
|                   | IN1              | 12       |
|                   | IN2              | 14       |
| Servo Motor       | Signal           | 3        |

---

## ⚙️ Software Structure

- `setup()`: Initializes display, buttons, SD card, and I2C devices  
- `loop()`: Handles button input and menu navigation  
- `drawPage()`: Renders current menu page  
- `readButtonsAndNavigate()`: Debounces and maps button presses  
- `handleButtonPress()`: Executes context-aware actions  
- `settings.h`: Contains SD card utilities and helper functions  

---

## 📁 Folder Structure

