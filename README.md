# Smart thermostat system, targeted for the STM32F401RE nucleo board
_Exercise for Microprocessors and Peripherals course, school of Electrical and Computer Engineering, Aristotle University of Thessaloniki._  
_June of 2020_

In this project we use the **STM32F401RE nucleo board** with an **ARM cortex-M4** processor. We also use a **DS18B20 Temperature Sensor**, a **HC-SR04 Ultrasonic distance sensor** and a **HD44780 LCD Display 16x2**. The systems monitors the room's temperature and displays it in the screen when somebody stands in front of it.

The project is built in C, using **ARM Keil** IDE. We also use the Arm University Project driver codes (in "drivers" path) as a level of abstraction for some core functionality. 
