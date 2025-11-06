🧠 ESP32 MultiWare System: Modular Sensor & Control Platform
This project showcases a modular ESP32-based system integrating multiple sensors and peripherals into a unified menu-driven interface. Designed for robotics, automation, and embedded experimentation, it features a navigable OLED UI, real-time sensor feedback, and SD card interaction.

📦 Key Features
OLED Menu Interface: Paginated UI with button navigation for selecting modules.

Multi-Sensor Integration: Ultrasonic, IR, MPU6050, and more.

Motor & Servo Control: Supports L298N motor driver and servo motor.

SD Card File System: View, create, and delete files via SPI.

Modular Architecture: Easily extendable with new modules and logic.

🛠️ Hardware Components
ESP32 Dev Board

SH1106 OLED Display (I2C)

HC-SR04 Ultrasonic Sensor

IR Transmitter & Receiver

MPU6050 Accelerometer/Gyroscope

L298N Motor Driver

Servo Motor

SD Card Module (SPI)

5x Push Buttons

LED (optional)

📐 Pin Mapping
Module	Function	GPIO Pin
Buttons	UP/DOWN/LEFT/RIGHT/SELECT	32, 33, 25, 26, 27
OLED & MPU6050	SDA/SCL (shared)	21, 22
IR Sensors	TX/RX	17, 16
Ultrasonic Sensor	TRIG/ECHO	4, 2
LED	Status Indicator	5
SD Card Module	MOSI/MISO/SCK/CS	23, 19, 18, 15
Motor Driver	ENA/IN1/IN2	13, 12, 14
Servo Motor	Control Signal	3
⚙️ Software Overview
setup(): Initializes display, buttons, SD card, and I2C devices.

loop(): Handles button input and menu navigation.

drawPage(): Renders current menu page.

readButtonsAndNavigate(): Debounces and maps button presses.

handleButtonPress(): Executes context-aware actions.

settings.h: External configuration and SD utilities.

📁 Repository Structure
Code
├── src/                  # Main Arduino sketch
├── settings.h            # SD card and file system utilities
├── assets/               # Wiring diagrams, photos, videos
├── docs/                 # Module documentation
└── README.md             # Project overview and setup guide
🚀 Getting Started
Clone the repository and open the sketch in Arduino IDE.

Install required libraries (U8g2, Wire, SPI, SD).

Upload to ESP32 and connect hardware.

Navigate the menu using buttons and explore modules.
