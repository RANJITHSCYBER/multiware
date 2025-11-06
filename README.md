🤖 ESP32 Autonomous Obstacle-Avoiding Robot
This project demonstrates a simple yet effective autonomous robot using an ESP32, HC-SR04 ultrasonic sensor, and L298N motor driver. The robot detects obstacles and avoids them by reversing and turning, using real-time sensor input and motor control.

📦 Features
Ultrasonic Distance Measurement: Detects obstacles using HC-SR04.

Motor Control via H-Bridge: Drives two DC motors with PWM and direction pins.

Obstacle Avoidance Logic: Stops, reverses, and turns when an object is detected.

Modular Functions: Clean structure for movement and sensor logic.

🛠️ Hardware Requirements
ESP32 microcontroller

HC-SR04 ultrasonic sensor

L298N motor driver module

2x DC motors

Power supply (battery or USB)

Jumper wires and breadboard (optional)

📐 Pin Configuration
Component	Pin Name	GPIO Pin
Ultrasonic Sensor	TRIG	5
Ultrasonic Sensor	ECHO	18
Motor A	ENA	23
Motor B	ENB	22
Motor A	IN1	16
Motor A	IN2	17
Motor B	IN3	19
Motor B	IN4	21
⚙️ Code Structure
setup()
Initializes serial communication.

Configures GPIO pins for motors and ultrasonic sensor.

loop()
Continuously checks distance.

Executes avoidance if obstacle is within 20 cm.

Moves forward if path is clear.

Sensor Function
getDistance(): Measures distance using ultrasonic sensor.

Movement Functions
moveForward(pwmA, pwmB)

moveBackward(pwmA, pwmB)

turnRight(pwmA, pwmB)

stopMotors()

Obstacle Handling
avoidObstacle():

Stop motors

Reverse briefly

Stop again

Turn right

Resume forward motion

🧠 Behavior Logic
If distance ≥ 20 cm → Move forward

If distance < 20 cm → Stop → Reverse → Turn → Resume

📈 Future Enhancements
Add left-turn logic

Use IR or bump sensors

Speed control based on proximity

Add Bluetooth/Wi-Fi for remote control

🧪 Testing Tips
Test ultrasonic sensor independently

Verify motor directions and PWM signals

Use serial output for debugging
