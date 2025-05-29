# TODO
- check battery charge capability
- fix schematic
- store readings and send via bluetooth
- calibrate light sensors
- fix lcd contrast (potential divider?)
- add battery pack and power switch
- design pcb
- design case
- make pcb
- print case
- assembleS


# Parts List
- stm32f446
- RG1602A (lcd screen)
- LDR + 82k resistor (light sensor)
- Capacitive moisture Sensor
- Potentiometer
- HC-06 (bluetooth)
- slide switch
- 9v Battery
	- TLV62569DBV (step down regulator)



# Schematic Fixes
	- should use Texas Instruments TLV62569DBV instead as is simpler
	- use 3.3v output as it will fluctuate
	- need inductor on converter circuit (bassicaly just follow datasheet)
	- find correct datasheet for the lcd. May need to have another buck converter to supply 5vs to the lcd
	- test capacitve moisture sensor and lcd with 3.3v supply 