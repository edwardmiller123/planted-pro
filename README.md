# TODO
- add nrst pin to schematic
- dont connect shield or on usb
- check battery charge capability
- calibrate light sensors (try stronger resistor)
- fix lcd contrast (potential divider?)
- add battery and switch, to schematic
- add charging circuit to schematic
- design pcb
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
- [LDR]()
- [Capacitive moisture Sensor](https://thepihut.com/products/capacitive-soil-moisture-sensor?variant=32137736421438&country=GB&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gQT=1)
- [Potentiometer](https://lcsc.com/product-detail/Potentiometers-Variable-Resistors_BOURNS-3296W-1-502LF_C60620.html?s_z=n_potentiometer)
- [HC-05 bluetooth module](https://www.ebay.co.uk/itm/292453360759?_trksid=p2332490.c101875.m1851&itmprp=cksum%3A2924533607597ce4980384724e938639b3fbd7e3c8a7%7Cenc%3AAQAKAAABgG96wQ16jds4VFcrhy1F3d4mbwZUJI9Fs%252BgdXYAHIzlX2e3YaNh7x%252BEnKA3G%252BCqSl1Xn4McfcWFK1GytmS2qxJ87mtE8Gm3iR1Ja4WBwh0hNHJrJx3Ki5mp04ow4CO7lP%252BooCybZDDU%252BbbSwmg7CbTin%252BBzBzbCYVnbjvyQAHu6--HI4MB7SvJl5IJqlyvomgoLMlgT6qAJzX0SANJhty2eH3mQTC7%252BUo1LCOV3zWqvDqzCovhNfZOVRWeQrp4Ib940QIXVfkbgMChVaQC8jaFp9qsYISBmoCevGwwiOSYfXSTaRtKKmKJomNeOFznaKU%252BfYJWIYT2d%252Byg8HwVOL%252FMvVKaBpjretD8cwgMV25JwAi%252F3okrkgASOdr8zgIhIJ1uhVPHUAAK4WAnLH4ByvQZw3r7reVYLkeqrS%252BgdrXDXqoDtmupxVLuJUaXoccZ%252BfDBpx%252FDfPTZwpnzQo33H4PQy87JFmpMNaG5D0nygs1lxubCCDXdalR9ajKwWFwMtcnA%253D%253D%7Campid%3APL_CLK%7Cclp%3A2332490&itmmeta=01JWEGA52AW853YMGK755K31X3) [datasheet](https://components101.com/sites/default/files/component_datasheet/HC-05%20Datasheet.pdf)
- [Slide switch](https://lcsc.com/product-detail/Slide-Switches_SHOU-HAN-MSK12C02-HB_C431541.html?s_z=s_Switches%257CSlide%2520Switches)
- [3.7v li-ion Battery](https://www.amazon.co.uk/Replacement-Rechargeable-Airplane-Doorbell-Handheld-No-Charger-Cable/dp/B0DCW3JZ81?source=ps-sl-shoppingads-lpcontext&ref_=fplfs&smid=A32BN3WEAFQHFN&gQT=1&th=1)
- [TLV62569DBV](https://www.lcsc.com/product-detail/DC-DC-Converters_Texas-Instruments-TLV62569DBVR_C141836.html?s_z=n_TLV62569DBV) (step down regulator) 
- [MCP73831T](https://lcsc.com/product-detail/Battery-Management_Microchip-Tech-MCP73831T-2ACI-OT_C424093.html?s_z=n_mcp73831) (power management chip for charging circuit)