# TODO
- Write message to screen when bluetooth connects
	- enable interrupt for PC7
- May need to change logic for ble
- Add battery sensor logic
- make pcb v2
- print case
- assemble

# Review notes
		
# Parts List
- Main board:
	- [STM32F446RC](https://lcsc.com/product-detail/Microcontrollers-MCU-MPU-SOC_STMicroelectronics-STM32F446RCT6_C915971.html?s_z=n_stm32f44)
	- [Photo resistor](https://lcsc.com/product-detail/Photoresistors_Senba-Sensing-Tech-GL5528_C125627.html?s_z=n_ldr)
	- [Potentiometer](https://www.lcsc.com/product-detail/C720648.html?s_z=n_potentiometer)
	- [Slide switch](https://www.amazon.com/dp/B08SLPWDR9?ref_=ppx_hzod_title_dt_b_fed_asin_title_0_1&th=1)
	- [2mm JST Battery Connector](https://www.amazon.com/CQRobot-Pieces-Connector-Housing-Adapter/dp/B09DP9FZTX?dib=eyJ2IjoiMSJ9.sNePKIuEPVewtteYfS58lZhW1LPmAVkOUnPIESSHRV1hvavSnGyljd52drfUkW5t4vHY770J1QYnJR6J5vuRVibrd9-3cRKNr1A3Vood9wdaIiXBvQhf0mdvStLx7khxwPU3F8Po8fU3Oj-s-p_wBD1qKetaDQ-Cf7YYYW11nkU2uM8TZcRV8bM0esdYdNTfc2DC5A6KGKV8WKj3iSCdm2a_5utHmO_mh-Fdf6InY6w.CRPhyqQcm5MJQbVlpLiQIjypTNYh30MgPEx-_YJeLQ0&dib_tag=se&keywords=jst%2Bph%2B2%2Bpin%2Bconnector&qid=1751377308&sr=8-2&th=1)
	- buck converter:
		- [TLV62569DBV](https://www.lcsc.com/product-detail/DC-DC-Converters_Texas-Instruments-TLV62569DBVR_C141836.html?s_z=n_TLV62569DBV) (step down regulator) 
	- battery charging:
		- [MCP73831T](https://lcsc.com/product-detail/Battery-Management_Microchip-Tech-MCP73831T-2ACI-OT_C424093.html?s_z=n_mcp73831) 
	- boost converter
		- [TPS613222A](https://www.lcsc.com/product-detail/C2071163.html?s_z=n_TPS613222A)
		- [Schottky diode](https://www.lcsc.com/product-detail/C8678.html?s_z=n_Schottky%2520diode)
	- Capacitors (0603) (all 16v)
		- [22uF (x1)](https://www.lcsc.com/product-detail/C6119868.html?s_z=s_Capacitors%257CMultilayer%2520Ceramic%2520Capacitors%2520MLCC%2520-%2520SMD%252FSMT)
		- [4.7uF (x5)](https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Samsung-Electro-Mechanics-CL10A475KO8NNNC_C19666.html?s_z=n_capacitors)
		- [10uF (x2)](https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Samsung-Electro-Mechanics-CL10A106MO8NQNC_C92487.html?s_z=n_capacitors)
		- [100nF (x6)](https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Samsung-Electro-Mechanics-CL10B104KO8NNNC_C66501.html?s_z=n_capacitors)
		- [120pF (x1)](https://www.lcsc.com/product-detail/C282671.html)
	- Resistors (0603)
		- [200k (x2)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html?s_z=n_resistors)
		- [2k (x1)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_FOJAN-FRC0603J202-TS_C2907137.html?s_z=n_resistors)
		- [1k (x1)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF1001T5E_C21190.html?s_z=n_resistors)
		- [1.5k (x2)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF1501T5E_C22843.html?s_z=s_Resistors%257CChip%2520Resistor%2520-%2520Surface%2520Mount)
		- [450k (x1)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF4533T5E_C25818.html?s_z=s_Resistors%257CChip%2520Resistor%2520-%2520Surface%2520Mount)
		- [100k (x3)](https://www.lcsc.com/product-detail/C3016388.html)
		- [1 (x1)](https://www.lcsc.com/product-detail/C2907116.html)
		- [5 (x1)](https://www.lcsc.com/product-detail/C2933231.html)
	- Inductors:
		- [2.2uH 1A Inductor (x2)](https://www.lcsc.com/product-detail/C365953.html?s_z=n_inductors)
	- Resonance filter:
		- [ferrite bead](https://www.lcsc.com/product-detail/C79382.html)
	- [USB-C Connector](https://lcsc.com/product-detail/USB-Connectors_SHOU-HAN-TYPE-C-6P-073_C668623.html?s_z=n_usb)

- External modules:
	- [RG1602A](https://lcsc.com/product-detail/LCD-Screen_HS-HS1602A-B_C18198249.html?s_z=s_Displays%257CLCD%2520Screen)
	- [Capacitive moisture Sensor](https://thepihut.com/products/capacitive-soil-moisture-sensor?variant=32137736421438&country=GB&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gQT=1)
	- [Vollgo VG6328A Bluetooth module](https://www.lcsc.com/product-detail/C20539408.html?s_z=s_IoT%252FCommunication%2520Modules%257CBluetooth%2520Modules)

- [4.2v lipo Battery](https://www.adafruit.com/product/2011)

	
# Dimensions
- main board: 60mm L, 48mm W
	- screws: 2mm Diam, centre is 3mm from edge
- lcd: 80mm L, 36mm W
	- screws: 3mm diameter
		- edge of screw hole 0.5mm by 0.5 from edge of screen
	- screen: 70mm L, 24mm W
	- screen to edge border: 5mm
- hc-05: 37mm L (43mm with pin headers), 15mm W
- hm-10: 38mm L, 16mm W
- Moisture sensor: 26 L (internal part), 23mm W, 1mm thick
- battery: 61mm L, 36mm W, 5mm H
- usbc connector: 8.64mm W, ~3mm H
	- 5.72mm away from board corner
- power switch: 19mm L, 6mm W
	- switch body: 10mm L
	- screw holes 2.2mm Diam 
- volgo bluetooth: 16mm L, 13.6mm W

- case: 
	- top and bottom screw holes are 2mm diam each starts 4.2mm from side.
	- window for charge light is 20mm L by 10mm W
	- lid is 5 mm deep, Lid indents start at 3.2mm from edge, indents are 3mm deep

- case v2:
	- top and bottom screw holes 2mm diam and are 3.2 mm from outer edges


- Plans:
 - Intergrate ble into main board
	- Could shift everything around and just about have room for volgo bluetooth module instead of ble header if hm-10 doesnt fit
 - may need to remove pin headers from hm-10s to make fit- 