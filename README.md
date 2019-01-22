## Meccanoid Personnal Robot Arduino PROJECT

### Overview

![alt text](https://github.com/sachaamm/meccanoidArduinoUnoShieldProject/blob/master/wikiPictures/meccanoid2.jpg)

youtube link

### Hardware

This is the hardware used on this project
- Arduino Esplora controller ( [available here](https://fr.shopping.rakuten.com/offer/buy/2201571401/gratuit-esplora-rocker-game-board-atmega32u4-module-support-robot-ecran-lcd-1-8-pouces-tft.html?bbaid=2974341335&t=180112&xtatc=PUB-%5Bggp%5D-%5BInformatique%5D-%5BAccessoires%5D-%5B2201571401%5D-%5Bneuf%5D-%5BYOINS%5D&ptnrid=pt%7C89152206803%7Cc%7C53434268603%7C2201571401&ptnrid=s24llCvmK_dc|pcrid|53434268603|pkw||pmt|&ja2=tsid:67590|cid:285670043|agid:14467128323|tid:aud-269143871707:pla-89152206803|crid:53434268603|nw:g|rnd:16272168161431935316|dvc:c|adp:1o1&gclid=Cj0KCQiAm5viBRD4ARIsADGUT25t6cqBpQwFY6DI_-hoSrfeVMcB9rYTRa3jL_OBDdxffjhCL8IMMwQaAhTVEALw_wcB) )
- 2x HM10 Bluetooth ( [available here](https://fr.shopping.rakuten.com/offer/buy/2552517283/bluetooth-4-0-pour-arduino-android-ios-hm-10-ble-cc2540-cc2541-serie-sans-fil-module.html?bbaid=3825848571&t=180176&xtatc=PUB-%5Bggp%5D-%5BInformatique%5D-%5BAccessoires%5D-%5B2552517283%5D-%5Bneuf%5D-%5BBuyCool%5D&ptnrid=pt%7C412904372812%7Cc%7C248309744840%7C2552517283&ptnrid=snHX2pR89_dc|pcrid|248309744840|pkw||pmt|&ja2=tsid:|cid:723021248|agid:58346345904|tid:aud-329787467942:pla-412904372812|crid:248309744840|nw:g|rnd:14070458870104494051|dvc:c|adp:1o1&gclid=Cj0KCQiAm5viBRD4ARIsADGUT24Wg6h3EPiis6JnYEjRWdm48go2qsXzKBhFZeI0HbhM2KkK3keFkXkaAp33EALw_wcB) )
- 2x Arduino UNO ( Eelego UNO ) ( [available here](https://fr.farnell.com/arduino-org/a000073/arduino-uno-cms-kit-de-dev/dp/2285200?gclid=Cj0KCQiAm5viBRD4ARIsADGUT27SB2p4XF8JkJ1D9ywi_7TdR8JGiINMsCUoFB-QVbYXcGUIP6FpGJEaAtopEALw_wcB&gross_price=true&mckv=nAfw6jTY_dc|pcrid|80993927942|&CAWELAID=120185620000343524&CAGPSPN=pla&CAAGID=13038120062&CMP=KNC-GFR-GEN-SHOPPING-2285200&CATCI=aud-127173563342:pla-89203456717) )
- 1x FourRelay-5V module ( [available here](https://www.banggood.com/fr/5V-4-Channel-Relay-Module-For-Arduino-PIC-ARM-DSP-AVR-MSP430-Blue-p-87987.html?gmcCountry=FR&currency=EUR&createTmp=1&utm_source=googleshopping&utm_medium=cpc_union&utm_content=2zou&utm_campaign=ssc-fr-all&gclid=Cj0KCQiAm5viBRD4ARIsADGUT25e2Ajjgxh5M218oXJrrltuOxtD6GfLqWbmM2E8d8yZ26lLdXyDNQsaAtivEALw_wcB&cur_warehouse=HK) )
- 1x FTDI Chip ( [available here](https://fr.banggood.com/FTDI-USB-OSD-Programmer-Module-for-F3-Flight-Control-AIO-Transmitter-OSD-BEC-Current-Sensor-p-1111287.html?akmClientCountry=FR&gmcCountry=FR&currency=EUR&createTmp=1&utm_source=googleshopping&utm_medium=cpc_bgcs&utm_content=zouzou&utm_campaign=pla-frg-toys-rctoys-pc&cur_warehouse=CN) )
- 1x 5V supply for Arduino B
- Wires / connector  / etc...


### Installation 
- Install Meccanoid Arduino Library ( [provided on this link](http://www.meccano.com/meccanoid-opensource) )
- Set up your Bluetooth slave-master pairing with an FTDI chip. One device will be Master and the other will be slave.
( [check this link](https://www.instructables.com/id/How-to-Use-Bluetooth-40-HM10/?fbclid=IwAR2GLLHkLqt9dNQLBvCo8Qc5vnz0IyvcvLNwDCA6tnjOxEIhA4G5cf7OB1w) )
- Upload the Arduino Source code on your devices, included in this repository in **Arduino/** 

   **EsploraBT3/** is for Esplora Controller
   
   **ReceiveBTEsplora_ArduinoRelay/** is for Arduino A
   
   **ReceiveBTESplora_Servos/** is for Arduino B
  
- Connect wires and devices following the schematic picture for your 2 Arduino and your HM10 device
![alt text](https://github.com/sachaamm/meccanoidArduinoUnoShieldProject/blob/master/wikiPictures/eagleSchematic.png)


- Connect HM10 master bluetooth device to your Esplora controller following this picture 
![alt text](https://github.com/sachaamm/meccanoidArduinoUnoShieldProject/blob/master/wikiPictures/91065-Arduino%20Esplora-Pic04.jpg)




### Instructions

- **Do not connect Meccanoid 5V supply GND to your Arduino B GND**, you will get a short-circuit on your HM10 bluetooth device which may destroy your device.
- **Do not upload your Arduino code when HM10-Serial connection is occuring** ( on the Arduino A, the HM10 device is connected to the Serial ports,
DIGITAL 0 (RX) and DIGITAL 1 (TX). 
- You may plug your motors to the HBridge in a different way than mine. If you get a mismatch on instructions directions, you can edit the Arduino B Code ( **ReceiveBTEsplora_ArduinoRelays** )
- Provide another 5V supply to your Arduino A using the USB port, because this device is controlling relays on a circuit supplied by the Meccanoid 5V supply, we have to supply this Arduino to another current source to avoid short circuits on the HBridge feet motors.

### Develop

- You can edit your own animations by editing the Arduino A Code ( **ReceiveBTEsplora_Servos** )

### STL Files

2 stl files for 3D printing are provided in order to grab easily your Esplora controller on hands

# Have Fun !

