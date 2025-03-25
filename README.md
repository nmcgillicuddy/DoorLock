# RFID Door Lock Security System
Personal project to create a working door lock system using ESP32s. Informal and for fun. Cheers

## Project Overview
Building a cool door lock using ESP32s! Going to make it smart, maybe with a web interface and app control. Aiming for something fun and useful, not a formal project. Any timeframes in this document don’t reflect the time put in. This is merely a side project.

The original idea of this project was to create a remote-controlled servo with web integration. The first stage of the system encompasses a servo working with an ESP32 S3 to monitor webserver endpoints that the microcontroller setup. Unlocking and locking actions are determined based on user inputs to those web points. Anything referring to stage 1 files is referring to that. As this project has progressed it has turned into an RFID-based security system. At the moment the webserver has been tossed aside and the current goal is to create an ESP-NOW locally function RFID system. The second stage system encompasses two microcontrollers working with each other to read RFID inputs and determine whether stored the user is valid or not based upon the credentials. 

** Project documentation: https://docs.google.com/document/d/1paIl5XaJ8KQLawIwF1z2wSQiaYhAy4koY_w7xShaVH4/edit?usp=sharing **

## Project Goals
- Experience with C++ and embedded systems
- Create a successful door locking system
- Master ESP32 use and communication protocols
- Attempt to stay academically active while at home
- Explore app integration
- Examine security concerns
