# Multiplexer-BNO055-IMU-Interface

This repository contains code for interfacing multiple BNO055 IMUs using multiple I2C multiplexers (TCA9548A) with an Arduino. The code initializes and reads data from multiple BNO055 sensors connected via the multiplexers and outputs the data through the serial port.

Working with the BNO055 sensor in conjunction with a multiplexer can be quite challenging due to its use of an I2C operation known as 'clock stretching'. Clock stretching allows the BNO055 to hold the clock line low to force the I2C master into a wait state, giving the sensor more time to process data. Unfortunately, this feature can cause compatibility issues with some microcontrollers and I2C multiplexers, leading to communication errors and inconsistent sensor readings.

During my development process, I encountered several issues related to clock stretching and found numerous pieces of information scattered across various online resources. However, I could not find a comprehensive guide that addressed all the challenges I faced. After extensive trial and error, I have developed a method that successfully integrates the BNO055 sensors with multiple I2C multiplexers. This repository contains the complete and functional code that worked for me. I hope it serves as a helpful reference for others who face similar challenges.

**This repository is intended for users who encounter one or more of the following issues:**

- **Arduino detects the I2C addresses but fails to initialize the BNO055 sensor**: The `bno.begin()` function returns `false`, indicating a problem with sensor initialization.
- **Sensor data appears as all zeros**: All readings from the BNO055 sensors are zero, suggesting potential issues with data communication or sensor configuration.
- **Identical sensor readings across different multiplexers**: Sensors connected to different multiplexers provide the same readings, which may indicate issues with multiplexer handling or sensor addressing.

## 📚 Table of Contents
- [Introduction](#introduction)
- [Key Points](#key-points)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

## 🚀 Introduction

This project demonstrates how to connect and read data from multiple BNO055 IMUs using multiple I2C multiplexers. The BNO055 is an advanced 9-axis orientation sensor, but it only supports two different I2C addresses. When more than two BNO055 sensors are needed, the TCA9548A multiplexer is required. This multiplexer allows you to communicate upto 8 I2C devices that have the same address.

## 🔑 Key Points

1. **Deactivate the Current Channel:** Before communicating with the next multiplexer, ensure you deactivate the channel on the current multiplexer. This can be done with the following code:
   
   ```cpp
   // TCA_addrs = [0x70, 0x71, 0x72...]
   
   for (uint8_t addr : TCA_addrs) {
     Wire.beginTransmission(addr);
     Wire.write(0);
     Wire.endTransmission();
   }

2. **Switch to a Dummy Channel**: After communicating with an active channel, switch to a dummy channel to ensure that no unintended data is transmitted or received. This prevents potential conflicts and helps in managing multiple sensors and multiplexers efficiently.

## ❓ How to Use This Repository

This repository is organized chronologically to reflect the development process. Each folder contains a wiring diagram and the corresponding code. While these examples use only one sensor per multiplexer, they offer a solid foundation and valuable insights for scaling up your setup.

- **For Beginners**: Start with the folder `1_one_TCA_one_BNO`. This example demonstrates the setup with a single TCA9548A multiplexer and one BNO055 sensor, providing a foundational understanding of wiring and code configuration.

- **For Intermediate Users**: Move to the folders `2_two_TCA_two_BNO` and `3_three_TCA_three_BNO` if you're comfortable with the basics and wish to explore more complex setups. Note that these examples include similar code blocks to enhance readability and help you grasp the concept.

- **For Advanced Users**: Explore the folder `4_generalised` for a refined code example that can be applied to as many multiplexers as needed, assuming they have different addresses. This folder provides a more generalized and scalable approach to handling multiple multiplexers.

## 🛠️ Hardware Requirements

- Arduino (e.g., Arduino Uno, Mega)
- Adafruit BNO055 IMU sensors
- TCA9548A I2C multiplexers
- Connecting wires

## 💻 Software Requirements

- Arduino IDE
- Adafruit Sensor Library
- Adafruit BNO055 Library

## ⚙️ Installation

1. Download and install the Arduino IDE from the [Arduino website](https://www.arduino.cc/en/software).
2. Install the Adafruit Sensor and Adafruit BNO055 libraries from the Arduino Library Manager.
3. Clone this repository or download the ZIP file and extract it.

## 💡 Usage

1. Open the Arduino IDE and load the provided `.ino` file.
2. Upload the code to your Arduino board.
3. Open the Serial Monitor from the Arduino IDE (set baud rate to 115200).
4. You should see the initialization and sensor data being printed on the Serial Monitor.

## 📝 License

This project is licensed under the MIT License.
