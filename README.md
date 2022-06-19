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
- 3 mosftet output, max 6A total accross 3 mosfet at any time
- low power features, turn off every peripheral except ESP32 that can deepsleep and LoRa
- LoRa (433Mhz) (not compatible with TTN :( 868 Mhz is too expensive) , wifi and bluetooth connectivity
- buzzer to locate the sensors (and maybe repell some animal ?)
- GPS to track the sensors and have an accurate time base
- local data display on OLED screen or remote via the LoRa
- input voltage monitoring, get an idea of the battery left if powered by one

![PCBB](https://user-images.githubusercontent.com/15912256/174489208-088df8cd-69f2-4267-9de7-8976912bf420.png)
![PCBA](https://user-images.githubusercontent.com/15912256/174489211-a9153713-dd13-4e30-a2ef-486b32c71e5a.png)

the PCB is 70 mm * 70 mm, i will add a better fixation system

## content
kicad schematic
BOM (20-40 CHF per unit, aliexpress as sole suplier (except PCB itself)

## futur works
- assemble and test the PCB once its manufactured
- write code to use it
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
