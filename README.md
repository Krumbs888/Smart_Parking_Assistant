# Smart Parking Assistant

An Arduino-based parking assistant designed to help drivers judge the distance between their vehicle and an obstacle while parking.

## what this is❓️

The Smart Parking Assistant is a personal Arduino project built to assist drivers when parking in an enclosed space, such as a garage

It makes use of an ultrasonic distance sensor to detect nearby obstacles and provides feedback through an LCD and buzzer. The project also includes a Proteus simulation for testing and debugging the circuit and program logic.

## 💡 Features
- **Distance Measurement** — Uses an ultrasonic sensor to measure the distance between the vehicle and the parking obstacle.
- **LCD Display** — Displays the measured distance in real time.
- **Visual & Audible Parking Feedback** — Three LEDs and a buzzer provide feedback based on how close the vehicle is to the desired parking distance.
- **Adjustable Parking Distance** — Includes a calibration mode that allows the target parking distance to be manually adjusted using a potentiometer
- **Calibration Mode Startup** — When entering calibration mode, the system displays a short startup interface that allows the user to either proceed with calibration or return to normal operation. If calibration is skipped, the existing target distance remains unchanged.

## 🛠 Hardware 
- Arduino UNO
- Ultrasonic sensor - HC-SR04
- 16x2 LCD
- Buzzer
- Potentiometer
- 3 LEDs (red, green, yellow)
- 3 220 ohm resistors

## 🖥 Software
- Arduino IDE
- Proteus Design Suite

## Limitations
- **Limited Sensor Capabilities** The system only measures the distance in front and cannot detect obstacles to the side of the vehicle.
- **Electrical Noise** - I did not have access to an external power source while building this, so all power is supplied from the arduino, causing some issues when reading from both the sensor and the potentiometer.

## Future Improvements
- **Automated Calibration** - Add a feature that allows the assistant to automatically suggest/set an optjmal target parking distance dependant on the size of the car the parking garage
- **Power Source** - Remove dependancy on The arduino board for power supply. A dedicated power supply will power will reduce electrical noise and also allow for a more portable build
-

## Contributions
All contributions are **greatly** appreciated.

## License
Distributed under the MIT License. See LICENSE for more info.
