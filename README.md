# FishTankGuardian

<p align="center">
    <img src="https://github.com/martinquattro/FishTankGuardian/blob/master/docs/images/ProjectSummary.png" alt="Summary">
</p>
FishTankGuardian is a project designed to automate the care of aquariums, featuring an automated feeding system that allows users to schedule specific feeding times for the fish. Additionally, it continuously monitors the water parameters and alerts the user if they exceed pre-established limits.
As a user interface, FishTankGuardian utilizes a Telegram bot, enabling users to interact with the system remotely.

#

<a href="https://github.com/martinquattro/FishTankGuardian/">GitHub Repository</a>

<a href="https://docs.google.com/document/d/1XdGEzkrNoZ_qmIqVganVuG-CD2JyYPGH2J27gbI3q3M/edit?usp=sharing">Memories and documentation (Spanish)</a>

<a href="https://www.youtube.com/watch?v=qyAfDcVljZQ">Video Summary</a>

<a href="https://www.youtube.com/watch?v=xCnJqBnb_as">Video Demonstration (Spanish)</a>

## Block Diagram
Four major peripheral groups are observed: user interface, sensors, real-time control, and actuators.

The first group consists of the Telegram Bot and an LCD display that will show relevant information on the screen for quick visualization. The second group includes two sensors: a temperature sensor and a total dissolved solids (TDS) sensor to monitor the water's condition. The third group involves an RTC module to maintain precise time control for time-based functions. The last group includes the actuator responsible for rotating the food container, dispensing a dose in each iteration.

Both the Telegram Bot and the real-time module utilize the WiFi module for message sending and receiving. The WiFi module is also used to synchronize the time via the internet.

<p align="center">
    <img width="600" src="https://github.com/martinquattro/FishTankGuardian/blob/master/docs/images/DiagramaBloques.jpg" alt="BlockDiagram">
</p>

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
<p align="center">
    <img width="700" src="https://github.com/martinquattro/FishTankGuardian/blob/master/docs/images/Schematic.png" alt="Schematic">
</p>

## Prototype
![ImagenPrototipo](https://github.com/martinquattro/FishTankGuardian/assets/62821432/0b167e03-a5f4-4a54-8494-378a07a9a657)
