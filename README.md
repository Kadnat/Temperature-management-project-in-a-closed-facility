# Design and Implementation of a Temperature Regulation System with Data Storage

## Introduction

Temperature regulation is essential in many fields. This project aims to design a temperature regulation system using a microcontroller, with heating and ventilation, to maintain a target temperature. The PIC18F4620 microcontroller is integrated for its processing capacity. The display is ensured by an SSD1306 screen and an I2C LCD, allowing clear and precise visualization of temperature data. Additionally, the microcontroller communicates with a computer via USART, facilitated by a Python script. This allows for the transmission of temperature data to the computer, enabling the extraction and analysis of data on a computer platform. This communication ensures remote monitoring and proactive management of the temperature regulation system, enhancing its efficiency and control.

![tempmast](https://github.com/Kadnat/Temperature-management-project-in-a-closed-facility/assets/126003868/65e94ca0-0bbc-4587-bccf-a45c75076b26)

## Components used

- **DS18B20 temperature probe**: A precise and versatile digital temperature sensor widely utilized in various applications. It boasts high accuracy of ±0.5°C within a temperature range of -10°C to +85°C, with a OneWire digital interface simplifying integration with microcontrollers.

- **L298N Controller**: A motor control module commonly employed to regulate the speed and direction of DC motors. It's utilized here to control the fan, facilitating system cooling.

- **5V 1 channel relay**: Utilized to power heating through a laboratory power supply via a relay controlled by the microcontroller.

- **AT24C32 EEPROM**: A type of non-volatile memory utilized for storing configuration data or other essential information, often interfaced with microcontrollers.

- **SD Card**: A storage device commonly used for data logging or storing large amounts of data in various projects.

- **OLED SSD1306**: A compact and versatile OLED display module, offering high contrast and low power consumption, ideal for displaying system parameters or status information.

- **LCD 1602A**: A popular alphanumeric LCD display module with 16x2 character configuration, suitable for displaying simple text information.

- **DS1307**: A real-time clock (RTC) module that provides accurate timekeeping and calendar functions, often used in systems requiring precise timing or scheduling.

- **Buzzer**: Utilized to produce audible alerts, such as when the temperature exceeds a set threshold, providing an audible indication of system status.

- **LED**: Used to indicate the system's state, with different states represented by varying LED behaviors, such as normal operation, too hot, or too cold.

## Data Storage

Two data storage devices are used: the AT24C32 EEPROM and the SD card. The AT24C32 EEPROM will be dedicated to saving temperature alarms, thus ensuring a quick response to critical events. On the other hand, the SD card will be used to store a complete history over a long period, offering the user easy and quick access to all recorded data.

## Display

Two display options are used to provide the user with a user-friendly and informative experience. An I2C 1602 LCD screen is used to display the date and time, and an SSD1306 screen is used to display the temperature in real time.

## History

A Python human-machine interface (HMI) has been developed to allow users to access various features based on their access rights. Users must authenticate by providing their identifiers. Three access levels are defined: Simple User, Supervisor, and Administrator.

- **Simple User**: This user can view the current temperature and the status of the system (heating, cooling, or stable). They can also access the history of temperature data but cannot make any changes to the system settings.

![User](https://github.com/Kadnat/Temperature-management-project-in-a-closed-facility/assets/126003868/1523813c-02d1-462a-b08c-71a2aa703915)


- **Supervisor**: In addition to the capabilities of a Simple User, the Supervisor can also access more detailed historical data, including alarm events.

![Supervisor](https://github.com/Kadnat/Temperature-management-project-in-a-closed-facility/assets/126003868/bcbf70cb-0b96-41d8-8d1e-2daa73a2a2af)


- **Administrator**: The Administrator has full control over the system. They can adjust all system parameters, including the target temperature range. They can also manage user accounts, adding or removing users and changing their access level. The Administrator has access to all historical data and can export this data for further analysis.

![Administrator](https://github.com/Kadnat/Temperature-management-project-in-a-closed-facility/assets/126003868/a84abc2f-3c9f-4805-abeb-88b532e4ddd1)


These different access levels ensure that the system can be used effectively and safely, with changes to system parameters controlled by authorized personnel. The history function allows all users to understand the performance of the system over time, while providing detailed data for troubleshooting and system optimization.

## Project Operation

The blue led means that the temperature is too low so the heating starts. If the temperature is too high, the led lights up in red and the fan does its cooling work. When the led lights up in green it means that the temperature corresponds to the expected one with an accuracy of 1 degree up or down.

![image](https://github.com/Kadnat/Temperature-management-project-in-a-closed-facility/assets/126003868/dd8ff8ac-f9e9-4e88-a08e-7c270ec6b02d)
![image](https://github.com/Kadnat/Temperature-management-project-in-a-closed-facility/assets/126003868/2a47f207-8834-449b-8ea0-0e11902d2ac8)
![green (1)](https://github.com/Kadnat/Temperature-management-project-in-a-closed-facility/assets/126003868/c96b5094-7275-4e15-8fae-370abbd94788)


## Link to the code

The code is located in : [Github](https://github.com/Kadnat/Temperature-management-project-in-a-closed-facility)

## Bibliography

- [DataSheet PIC](https://www.microchip.com/en-us/product/pic18f4620)
- [Led Screen](https://www.digikey.com/htmldatasheets/production/2047793/0/0/1/ssd1306.html)
- [I2C Module LCD](https://handsontec.com/dataspecs/module/I2C_1602_LCD.pdf)
- [DataSheet L298N](https://www.alldatasheet.com/datasheet-pdf/pdf/22440/STMICROELECTRONICS/L298N.html)
- [AT24C32 EEPROM](https://ww1.microchip.com/downloads/en/DeviceDoc/doc0336.pdf)
