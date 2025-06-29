# TODO
- check battery charge capability
- calibrate light sensors (try stronger resistor)
- fix lcd contrast (potential divider?)
- design case
- make pcb
- print case
- assemble

- optional:
	- replaces all queue swith ring buffers
	- send current reading along with historical data
	- receive current time?


# Parts List
- [STM32F446RE](https://www.lcsc.com/product-detail/Microcontrollers-MCU-MPU-SOC_STMicroelectronics-STM32F446RET6_C69336.html?s_z=n_stm32f446re) 
- RG1602A (lcd screen) 
- [Photo resistor](https://lcsc.com/product-detail/Photoresistors_Senba-Sensing-Tech-GL5528_C125627.html?s_z=n_ldr)
- [Capacitive moisture Sensor](https://thepihut.com/products/capacitive-soil-moisture-sensor?variant=32137736421438&country=GB&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gQT=1)
- [Potentiometer](https://lcsc.com/product-detail/Potentiometers-Variable-Resistors_BOURNS-3296W-1-502LF_C60620.html?s_z=n_potentiometer)
- [HC-05 bluetooth module](https://www.ebay.co.uk/itm/292453360759?_trksid=p2332490.c101875.m1851&itmprp=cksum%3A2924533607597ce4980384724e938639b3fbd7e3c8a7%7Cenc%3AAQAKAAABgG96wQ16jds4VFcrhy1F3d4mbwZUJI9Fs%252BgdXYAHIzlX2e3YaNh7x%252BEnKA3G%252BCqSl1Xn4McfcWFK1GytmS2qxJ87mtE8Gm3iR1Ja4WBwh0hNHJrJx3Ki5mp04ow4CO7lP%252BooCybZDDU%252BbbSwmg7CbTin%252BBzBzbCYVnbjvyQAHu6--HI4MB7SvJl5IJqlyvomgoLMlgT6qAJzX0SANJhty2eH3mQTC7%252BUo1LCOV3zWqvDqzCovhNfZOVRWeQrp4Ib940QIXVfkbgMChVaQC8jaFp9qsYISBmoCevGwwiOSYfXSTaRtKKmKJomNeOFznaKU%252BfYJWIYT2d%252Byg8HwVOL%252FMvVKaBpjretD8cwgMV25JwAi%252F3okrkgASOdr8zgIhIJ1uhVPHUAAK4WAnLH4ByvQZw3r7reVYLkeqrS%252BgdrXDXqoDtmupxVLuJUaXoccZ%252BfDBpx%252FDfPTZwpnzQo33H4PQy87JFmpMNaG5D0nygs1lxubCCDXdalR9ajKwWFwMtcnA%253D%253D%7Campid%3APL_CLK%7Cclp%3A2332490&itmmeta=01JWEGA52AW853YMGK755K31X3) [datasheet](https://components101.com/sites/default/files/component_datasheet/HC-05%20Datasheet.pdf)
- [Slide switch](https://lcsc.com/product-detail/Slide-Switches_SHOU-HAN-MSK12C02-HB_C431541.html?s_z=s_Switches%257CSlide%2520Switches)
	- [maybe this one](https://www.amazon.co.uk/Miniature-Model-Railway-Switch-2-Position/dp/B00TXNXFZC/259-1494948-4487924?pd_rd_w=fCK5e&content-id=amzn1.sym.6448d24d-f04d-4e11-90e5-10165d266aa2&pf_rd_p=6448d24d-f04d-4e11-90e5-10165d266aa2&pf_rd_r=H9Y4PVHCB2WFH620DCAF&pd_rd_wg=IW9Gh&pd_rd_r=92e969ec-19a7-4c8d-8360-4e458b67bc73&pd_rd_i=B00TXNXFZC&psc=1)
- [3.7v lipo Battery](https://www.adafruit.com/product/2011)
- [battery connector](https://uk.farnell.com/jst-japan-solderless-terminals/b2b-ph-k-s-lf-sn/header-top-entry-2way-2mm/dp/9491856)
- [TLV62569DBV](https://www.lcsc.com/product-detail/DC-DC-Converters_Texas-Instruments-TLV62569DBVR_C141836.html?s_z=n_TLV62569DBV) (step down regulator) 
- [MCP73831T](https://lcsc.com/product-detail/Battery-Management_Microchip-Tech-MCP73831T-2ACI-OT_C424093.html?s_z=n_mcp73831) (power management chip for charging circuit)
- Capacitors (0603)
	- 4.7uF (x4)
	- 10uF (x1)
	- 100nF (x6)
- Resistors (0603)
	- [900k (x1)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF9003T5E_C407504.html?s_z=n_resistors)
	- [200k (x2)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html?s_z=n_resistors)
	- [2k (x1)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_FOJAN-FRC0603J202-TS_C2907137.html?s_z=n_resistors)
	- [1k (x1)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF1001T5E_C21190.html?s_z=n_resistors)
	- [1.5k (x2)](https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF1001T5E_C21190.html?s_z=n_resistors)
- [2.2uH Inductor (0603)](https://lcsc.com/product-detail/Inductors-SMD_Murata-Electronics-LQM18PN2R2MFRL_C337910.html?s_z=n_inductor)