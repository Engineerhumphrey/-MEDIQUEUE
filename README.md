# -MEDIQUEUE
SMART HOSPITAL QUEUE MANAGEMENT SYSTEM WITH TRIAGE AND SMS NOTIFICATION

## Overview

The Smart Hospital Queue Management System is a microcontroller-based solution designed to improve patient flow and reduce waiting time in hospitals.

The system automatically registers patients,

determines their triage status based on selected symptoms, 

generates a queue token, 

and sends SMS notifications to keep patients informed about their position in the queue.


The system is built using

an Arduino Uno and communicates with patients through SMS using the SIM800L GSM Module.

A 16x2 LCD Display 

and keypad provide a simple user interface for nurses and patients.


## Project Objectives


To automate patient queue management in hospitals.


To reduce congestion and waiting times in hospital reception areas.


To automatically determine patient triage levels based on symptoms.


To notify patients through SMS when their turn approaches.


To create a low-cost hospital queue management prototype using embedded systems.


## System Features


Patient phone number entry using a keypad


Automatic symptom-based triage determination


Token generation for queue management


SMS notification system


Nurse interface using keypad controls


Buzzer alerts for queue updates


LCD display for real-time system feedback


## System Workflow


Nurse selects Register Patient using the keypad.


Patient enters their phone number.


Patient selects their symptoms.


The system evaluates symptoms and determines the triage category.


A queue token number is generated.


An SMS confirmation is sent to the patient.


When the nurse calls the next patient, the system sends:


A warning message when two patients remain ahead.


A turn notification when it is the patient's turn.


## Hardware Components


The system uses the following hardware components:


Arduino Uno – main controller


SIM800L GSM Module – SMS communication


16x2 LCD Display – user interface display


3×4 Matrix Keypad – input interface


Buzzer – alert notifications


Lithium Battery – GSM power supply


External power supply – system power


Capacitor – stabilizes GSM module power


## Software Requirements


Arduino IDE


Required Arduino Libraries:
LiquidCrystal_I2C


Keypad


SoftwareSerial


## System Architecture


The system operates using a microcontroller-based architecture:


Patient Input → Arduino Processing → Triage Decision → Token Generation → SMS Notification → Queue Monitoring


## Installation and Setup


Install Arduino IDE.

Install the required libraries:


LiquidCrystal_I2C


Keypad


SoftwareSerial


Upload the project code to the Arduino Uno.



Connect all hardware components according to the circuit diagram.



Insert a working SIM card into the SIM800L GSM module.



Power the system and begin patient registration.


## Example SMS Notification


Example message sent to patients:


Hospital Queue System


Token: 12


Triage: URGENT


Please wait for your turn.


Queue update message:

Queue Update


Two patients remaining before your turn.


Please prepare.


Turn notification:


Queue Alert


It is now your turn.


Please proceed to the consultation room.


## Future Improvements
Possible future improvements include:


Integration with hospital management systems (HIS)


Mobile application for queue monitoring


Online patient pre-registration


Real-time queue dashboard


Integration with vital-sign sensors for automated triage


## Author
Humphrey Kombe Nzai


Diploma in Mechatronics Engineering


Taita Taveta National Polytechnic

## License
This project is for educational and research purposes.

