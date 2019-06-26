# ARC_Project
## INTRODUCTION
This project is aimed at implementing **Online Driver State Monitoring** to check for Driving Under Influence(DUI) and to take appropriate actions for alleviating the potential dangers associated with a drunk driver on the street. This project makes use of the ARC-IOTDK as the main processor and the _MQ-3 alcohol gas sensor_ to sense for **Blood Alcohol Concentration(BAC)**. A BAC of 0.08% indicates that the driver is drunk and it is not safe for him/her to drive. Drunk Driving is a menace to the society and this project came up as a solution to this ever-increasing menace. As mentioned before, the gas sensor's output shall give an indication of the intoxication of the driver. To increase exactness and reliability, this project shall also make use of _face detection algorithm_ to detect for driver change, and certain facial features like colour of eyes, how far is the driver's eye opening etc. This process is repeated after defined times to provide continuous and uninterrupted monitoring. Repeated offenders should be penalized and that is why upon violation of rules, reports shall be sent to authorities via the appropriate messaging/texting platform.

### H/W Setup
The Hardware requirements are as below:
*DesignWare ARC IoT Development Kit
*MQ3 alcohol sensor
*Camera 
*Circuit Design Components(Resistors, Capacitors)
*Alarms 
*Battery pack
*16x2 LCD
*DC Motors  

### S/W Setup
The Software requirements are as below:
*embARC Open Source Platform
*MetaWare Development Toolkit & GNU Toolchain

This project has certain challenges like mounting of camera at the right location. The location of the camera is very important since the image being captured depends entirely on where the camera is placed. The camera should be placed such that the entire face of the driver be captured in the images for us to take reliable inferences. The positioning of the sensor is also critical as it's placement should not hinder the driver in any way but at the same time be at a position so that it can sense the breath exhaled by the driver in its entirety. 
