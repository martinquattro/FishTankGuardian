# FishTankGuardian

![logo](https://github.com/martinquattro/FishTankGuardian/assets/62821432/8e360b3e-76d7-4d51-9f2c-2b49c78eac6d)

FishTankGuardian is a project designed to automate the care of aquariums, developed as part of the Embedded Systems course at the Faculty of Engineering of UBA. This system, implemented in C++ using an ARM board, offers various functionalities to facilitate aquarium management.
The project features an automated feeding system that allows users to schedule specific feeding times for the fish. Additionally, it continuously monitors the water parameters and alerts the user if they exceed pre-established limits.
As a user interface, FishTankGuardian utilizes a Telegram bot, enabling users to interact with the system remotely. This feature allows users to receive updates on the aquarium's status, adjust feeding schedules, and receive instant alerts in case of issues.

Resository: https://github.com/martinquattro/FishTankGuardian/

Memories and documentation (Spanish) https://docs.google.com/document/d/1XdGEzkrNoZ_qmIqVganVuG-CD2JyYPGH2J27gbI3q3M/edit

_________________________________________________________________________________________________________

## Block Diagram
Four major peripheral groups are observed: user interface, sensors, real-time control, and actuators.

The first group consists of the Telegram Bot and an LCD display that will show relevant information on the screen for quick visualization. The second group includes two sensors: a temperature sensor and a total dissolved solids (TDS) sensor to monitor the water's condition. The third group involves an RTC module to maintain precise time control for time-based functions. The last group includes the actuator responsible for rotating the food container, dispensing a dose in each iteration.

Both the Telegram Bot and the real-time module utilize the WiFi module for message sending and receiving. The WiFi module is also used to synchronize the time via the internet.

![DiagramaBloques](https://github.com/martinquattro/FishTankGuardian/assets/62821432/fed1e7d5-a0e3-467d-844b-3e32a0abc877)

## Firmware diagram

![DiagramaDeFirmware](https://github.com/martinquattro/FishTankGuardian/assets/62821432/0975e33d-24b8-4cd8-bb26-b1094bc61246)

## List of components
- NUCLEO-F429ZI
- Display LCD 20x4
- ESP32 WROOM
- Driver ULN 2003
- Motor 28BYJ-48
- Power Suppley 12 V
- RTC Tiny
- TDS Sensor
- Potentiometer (replacing temperature sensor)
  
## Connections Diagram 
![Esquematico](https://github.com/martinquattro/FishTankGuardian/assets/62821432/1703425e-3ddb-49a6-a296-e08fc318a3e5)

## Prototype
![ImagenPrototipo](https://github.com/martinquattro/FishTankGuardian/assets/62821432/0b167e03-a5f4-4a54-8494-378a07a9a657)
