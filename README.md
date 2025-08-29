# TODO
- order HM-10
- order parts for pcb v2
- make pcb v2
- print case
- assemble

# Review notes

# Parts List
- main board:
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
		- [10uF (x1)](https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Samsung-Electro-Mechanics-CL10A106MO8NQNC_C92487.html?s_z=n_capacitors)
		- [100nF (x6)](https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Samsung-Electro-Mechanics-CL10B104KO8NNNC_C66501.html?s_z=n_capacitors)
		- [120pF (x1)](https://www.lcsc.com/product-detail/C282671.html)
	- Resistors (0603)
		- [200k (x2)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html?s_z=n_resistors)
		- [2k (x1)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_FOJAN-FRC0603J202-TS_C2907137.html?s_z=n_resistors)
		- [1k (x1)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF1001T5E_C21190.html?s_z=n_resistors)
		- [1.5k (x2)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF1501T5E_C22843.html?s_z=s_Resistors%257CChip%2520Resistor%2520-%2520Surface%2520Mount)
		- [450k (x1)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF4533T5E_C25818.html?s_z=s_Resistors%257CChip%2520Resistor%2520-%2520Surface%2520Mount) backup
		- [100k (x1)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0402WGF1003TCE_C25741.html?s_z=n_resistors)
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
	- [HC-05 bluetooth module](https://www.ebay.co.uk/itm/292453360759?_trksid=p2332490.c101875.m1851&itmprp=cksum%3A2924533607597ce4980384724e938639b3fbd7e3c8a7%7Cenc%3AAQAKAAABgG96wQ16jds4VFcrhy1F3d4mbwZUJI9Fs%252BgdXYAHIzlX2e3YaNh7x%252BEnKA3G%252BCqSl1Xn4McfcWFK1GytmS2qxJ87mtE8Gm3iR1Ja4WBwh0hNHJrJx3Ki5mp04ow4CO7lP%252BooCybZDDU%252BbbSwmg7CbTin%252BBzBzbCYVnbjvyQAHu6--HI4MB7SvJl5IJqlyvomgoLMlgT6qAJzX0SANJhty2eH3mQTC7%252BUo1LCOV3zWqvDqzCovhNfZOVRWeQrp4Ib940QIXVfkbgMChVaQC8jaFp9qsYISBmoCevGwwiOSYfXSTaRtKKmKJomNeOFznaKU%252BfYJWIYT2d%252Byg8HwVOL%252FMvVKaBpjretD8cwgMV25JwAi%252F3okrkgASOdr8zgIhIJ1uhVPHUAAK4WAnLH4ByvQZw3r7reVYLkeqrS%252BgdrXDXqoDtmupxVLuJUaXoccZ%252BfDBpx%252FDfPTZwpnzQo33H4PQy87JFmpMNaG5D0nygs1lxubCCDXdalR9ajKwWFwMtcnA%253D%253D%7Campid%3APL_CLK%7Cclp%3A2332490&itmmeta=01JWEGA52AW853YMGK755K31X3) [datasheet](https://components101.com/sites/default/files/component_datasheet/HC-05%20Datasheet.pdf)
		- [HM-10](https://www.aliexpress.com/item/1005009402343096.html?spm=a2g0o.productlist.main.2.320b7c96oQV4Gw&algo_pvid=ebedd72d-44bf-445e-82d3-dead5112c31a&algo_exp_id=ebedd72d-44bf-445e-82d3-dead5112c31a-1&pdp_ext_f=%7B%22order%22%3A%222%22%2C%22eval%22%3A%221%22%7D&pdp_npi=6%40dis%21GBP%214.94%212.47%21%21%2146.51%2123.26%21%40211b61a417551015548667805ee04a%2112000048989294355%21sea%21UK%210%21ABX%211%210%21n_tag%3A-29910%3Bm03_new_user%3A-29895&curPageLogUid=lp460BPBusNW&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005009402343096%7C_p_origin_prod%3A)

- [4.2v lipo Battery](https://www.adafruit.com/product/2011)

	
# Dimensions
- main board: 60mm L, 48mm W
	- screws: 2mm Diam, centre is 3mm from edge
- lcd: 80mm L, 36mm W
	- screws: 3mm diameter
		- edge of screw hole 0.5mm by 0.5 from edge of screen
	- screen: 70mm L, 24mm W
	- screen to edge border: 5mm
- hc-05: 37mm L, 15mm W
- Moisture sensor: 26 L (internal part), 23mm W, 1mm thick
- battery: 61mm L, 36mm W, 5mm H
- usbc connector: 8.64mm W, ~3mm H
	- 5.72mm away from board corner
- power switch: 19mm L, 6mm W
	- switch body: 10mm L
	- screw holes 2.2mm Diam 

- case: 
	- top and bottom screw holes are 2mm diam each starts 4.2mm from side.
	- window for charge light is 21mm from edge of lid and is 20mm L by 10mm W
	- lid is 5 mm deep, Lid indents start at 3.2mm from edge, indents are 3mm deep