# multiware
🧭 ESP32 Multi-Sensor Menu System with OLED Display
This project implements a minimal menu-driven interface for an ESP32-based embedded system using an OLED display, multiple sensors, and peripherals. It features a navigable UI controlled by five buttons and supports modules like SD card, ultrasonic sensor, IR sensors, motor drivers, and servo motors.

📋 Features
OLED Menu UI: Paginated menu with 2x2 layout and multiple pages.

Button Navigation: Up, Down, Left, Right, and Select buttons with debounce logic.

SD Card Integration: File listing, deletion, and creation via SPI.

Modular Sensor Support: IR, ultrasonic, MPU6050, and servo motor.

Status Feedback: Transient messages for user actions.

🛠️ Hardware Components
ESP32 Dev Board

SH1106 OLED Display (I2C)

HC-SR04 Ultrasonic Sensor

IR Transmitter and Receiver

L298N Motor Driver

Servo Motor

SD Card Module (SPI)

5x Push Buttons

LED (optional for status)

📐 Pin Configuration
Component	Function	GPIO Pin
Buttons	UP/DOWN/LEFT/RIGHT/SELECT	32, 33, 25, 26, 27
OLED & MPU6050	SDA/SCL (shared)	21, 22
IR Sensors	TX/RX	17, 16
Ultrasonic	TRIG/ECHO	4, 2
LED	Status Indicator	5
SD Card Module	MOSI/MISO/SCK/CS	23, 19, 18, 15
Motor Driver	ENA/IN1/IN2	13, 12, 14
Servo Motor	Control Signal	3
⚙️ Code Structure
setup(): Initializes display, buttons, SD card, and I2C.

loop(): Handles button input and menu navigation.

readButtonsAndNavigate(): Debounces and maps button presses.

handleButtonPress(): Context-aware navigation and actions.

drawPage(): Renders current menu page on OLED.

settings.h: External configuration and SD card utilities.

📈 Future Enhancements
Add sensor data visualization (e.g., MPU6050 readings).

Implement Wi-Fi setup and web interface.

Add file viewer/editor for SD card contents.

Expand menu with real-time telemetry and control options.
