# ARC_Project
## INTRODUCTION
This project titled **DUI Monitoring System** is aimed at implementing a DUI detection system for vehicles which checks for alcohol consumption and detects drowsiness in Drivers and seeks to provide suitable alerts and react accordingly. This project makes use of the ARC-IOTDK as the main processor and the _MQ-3 alcohol gas sensor_ to sense for **Blood Alcohol Concentration(BAC)**. A BAC of 0.08% indicates that the driver is drunk and it is not safe for him/her to drive. 
The webcam monitors the driver and raises an alert when the driver is caught drowsy. If the driver is caught being drowsy repeatedly, an eamil is sent right away to the closest/respective authority.

### H/W Setup
The Hardware requirements are as below:
- DesignWare ARC IoT Development Kit
- MQ3 alcohol sensor
- Camera 
- Circuit Design Components(Resistors, LEDs)
- Battery pack
- DC Motors (BO 100rpm)

### S/W Setup
The Software requirements are as below:
- embARC Open Source Platform
- MetaWare Development Toolkit & GNU Toolchain
- Python & OpenCV 
- Putty
- Digilent Adept

This project has certain challenges like mounting of camera at the right location. The configuration of the ARC IoTDK could also be explored further - like processing the image recognition algorithm directly on the processor.
