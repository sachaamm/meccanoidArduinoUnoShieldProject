## ARDUINO SHIELD FOR MECCANOID PERSONNAL ROBOT PROJECT

### Overview

Schematic
Meccanoid robot
youtube link


### Installation
- Install Meccanoid Arduino Library ( [provided on this link](http://www.meccano.com/meccanoid-opensource) )
- Set up your Bluetooth slave-master pairing with an FTDI chip. One device will be Master and the other will be slave.
( [check this link](https://www.instructables.com/id/How-to-Use-Bluetooth-40-HM10/?fbclid=IwAR2GLLHkLqt9dNQLBvCo8Qc5vnz0IyvcvLNwDCA6tnjOxEIhA4G5cf7OB1w) )
- Upload the Arduino Source code on your devices, included in this repository in **Arduino/** 

   **EsploraBT3/** is for Esplora Controller
   
   **ReceiveBTEsplora_ArduinoRelay/** is for Arduino A
   
   **ReceiveBTESplora_Servos/** is for Arduino B
  
- Connect wires following the schematic picture for your 2 Arduino and your HM10 device
![alt text](https://github.com/sachaamm/meccanoidArduinoUnoShieldProject/blob/master/wikiPictures/eagle_2019-01-22_17-27-22.png)


- Connect HM10 master bluetooth device to your Esplora controller following this picture 


### Hardware

This is the hardware used on this project
- Arduino Esplora controller
- 2x HM10 Bluetooth 
- 2x Arduino UNO ( Eelego UNO )
- 1 FourRelay-5V module ( [available here](https://www.banggood.com/fr/5V-4-Channel-Relay-Module-For-Arduino-PIC-ARM-DSP-AVR-MSP430-Blue-p-87987.html?gmcCountry=FR&currency=EUR&createTmp=1&utm_source=googleshopping&utm_medium=cpc_union&utm_content=2zou&utm_campaign=ssc-fr-all&gclid=Cj0KCQiAm5viBRD4ARIsADGUT25e2Ajjgxh5M218oXJrrltuOxtD6GfLqWbmM2E8d8yZ26lLdXyDNQsaAtivEALw_wcB&cur_warehouse=HK) )
- FTDI Chip

### Instructions

- Do not upload your Arduino code when Serial connection is occuring ( on the Arduino A, the HM10 device is connected to the Serial ports,
DIGITAL 0 (RX) and DIGITAL 1 (TX). 
- Do not connect Meccanoid 5V supply to your Arduino B GND, you will get a short-circuit on your HM10 bluetooth device which may destroy your device.
- Provide another 5V supply to your Arduino A using the USB port, because this device is controlling relays on a circuit supplied by the Meccanoid 5V supply, we have to supply this Arduino to another current source to avoid short circuits on the HBridge feet motors.
- You may plug your motors to the HBridge in a different way than mine. If you get a mismatch on instructions directions, you can edit the Arduino B Code ( ReceiveBTEsplora_ArduinoRelays )

### Develop

- You can edit your

### STL Files

2 stl files for 3D printing are provided in order to grab easily your Esplora controller on hands



