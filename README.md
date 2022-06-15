# PlantSensor
Sensors node to monitor ambient light, temperature, humitidy, noise and soil moisture.
It it also able to drive a water pump and two light and have a LoRa transceiver to hopefully prevent my house plants from dying (among other use)
it was designed with a few use case in mind : indoor plant, outdoor plant, weather station, mailbox notifier, environemental sensor (3 I2C port that can be used for with sensors @ 3V3)
it is low power (when en deepsleep) (current draw under 50 uA@3V7 and even less at 12V)

## Features
- 3 I2C port for 3V3 peripheral (designed for OLED display, lux meter, pressure sensors)
- ambiant humidity sensors and temperature (DHT11)
- soil humidity sensors (capacitive module with analog out)
- lux meter
- pressure sensor
- microhone (detect noise level + weather / rain / wind ?)
- 3 mosftet outpout, each up to 2A. 24W @12V (need to make a thicker trace)
- low power features, turn off every peripheral except ESP32 that can deepsleep and LoRa
- LoRa (433Mhz) (not compatible with TTN :( 868 Mhz is too expensive) , wifi and bluetooth connectivity
- buzzer to locate the sensors (and maybe repell some animal ?)
- GPS to track the sensors and have an accurate time base
- local data display on OLED screen or remote via the LoRa
- input voltage monitoring, get an idea of the battery left if powered by one

![image](https://user-images.githubusercontent.com/15912256/173457969-1ef0dad0-d3d3-4e9f-94d5-d504dadfc579.png)

the PCB is 70 mm * 70 mm, i will add a better fixation system (mouting hole + some space for fixation bracket)

![image](https://user-images.githubusercontent.com/15912256/173710470-190e3228-3d07-4be2-9e39-55b368796fda.png)

## content
kicad schematic
BOM (20-30 CHF per unit, aliexpress as sole suplier)

## futur works
- assemble and test the PCB once its manufactured
- write code to use it
- fix 12V trace that is too thin
- add snubber circuit on the mosfet (or ZFS diode)
- check current of the buck converter in low power mode

## firmware 
whenever the CPU come out of the sleep mode one of the following action should be performed :

mode 1)
read the config
read the sensors specified in the config
take the decision to turn on or off the mosfet if the condition set in the config are set
take the decision to display the data as specified in the config
send the data to a server if set in the config
retreive config update from the server if any and update own config or pass it to the next node

mode 2)
just transmit to any exit node the data passed uppon
applie the insctruction if we are the destination node

comming out of sleep mode are set in the config
- button action
- timer
- LoRa event

which system they trigger should also be specified in the config
