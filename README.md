# Design and Implementation of a Temperature Regulation System with Data Storage

## Introduction

Temperature regulation is essential in many fields. This project aims to design a temperature regulation system using a microcontroller, with heating and ventilation, to maintain a target temperature. The PIC18F4620 microcontroller is integrated for its processing capacity and real-time control features. The display is ensured by an SSD1306 screen and an I2C LCD, allowing a clear and precise visualization of temperature data. The temperature data is also transmitted to a computer via a Python script, thus allowing the extraction and analysis of data on a computer platform, ensuring remote monitoring and proactive management of the temperature regulation system.

## Components used

- **DS18B20 temperature probe**: A precise and versatile digital temperature sensor, widely used in various applications. With its high accuracy of ±0.5°C in a temperature range of -10°C to +85°C, its OneWire digital interface simplifies its integration with microcontrollers.
- **L298N Controller**: A motor control module commonly used to control the speed and direction of DC motors. It is used to control the fan allowing the system to cool down.
- **5V 1 channel relay**: Used to power the heating through a laboratory power supply via a relay controlled by the microcontroller.

## Data Storage

Two data storage devices are used: the AT24C32 EEPROM and the SD card. The AT24C32 EEPROM will be dedicated to saving temperature alarms, thus ensuring a quick response to critical events. On the other hand, the SD card will be used to store a complete history over a long period, offering the user easy and quick access to all recorded data.

## Display

Two display options are used to provide the user with a user-friendly and informative experience. An I2C 1602 LCD screen is used to display the date and time, and an SSD1306 screen is used to display the temperature in real time.

## History

A Python human-machine interface (HMI) has been developed to allow users to access various features based on their access rights. Users must authenticate by providing their identifiers. Three access levels are defined: Simple User, Supervisor, and Administrator.

- **Simple User**: This user can view the current temperature and the status of the system (heating, cooling, or stable). They can also access the history of temperature data but cannot make any changes to the system settings.

- **Supervisor**: In addition to the capabilities of a Simple User, the Supervisor can adjust the target temperature within a predefined range. They can also access more detailed historical data, including alarm events.

- **Administrator**: The Administrator has full control over the system. They can adjust all system parameters, including the target temperature range. They can also manage user accounts, adding or removing users and changing their access level. The Administrator has access to all historical data and can export this data for further analysis.

These different access levels ensure that the system can be used effectively and safely, with changes to system parameters controlled by authorized personnel. The history function allows all users to understand the performance of the system over time, while providing detailed data for troubleshooting and system optimization.

## Project Operation

The blue led means that the temperature is too low so the heating starts. If the temperature is too high, the led lights up in red and the fan does its cooling work. When the led lights up in green it means that the temperature corresponds to the expected one with an accuracy of 1 degree up or down.

## Link to the code

The code is located in : [Github](https://github.com/Kadnat/Temperature-management-project-in-a-closed-facility)

## Bibliography

- [DataSheet PIC](https://www.microchip.com/en-us/product/pic18f4620)
- [Led Screen](https://www.digikey.com/htmldatasheets/production/2047793/0/0/1/ssd1306.html)
- Datasheet of the I2C 1602 LCD in pdf
- Darasheet L298N STmicroelectronics in pdf