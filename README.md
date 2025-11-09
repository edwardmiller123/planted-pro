# planted-pro
This is the planted-pro plant monitor. It is my first electronics project and the goal was to create a full product end to end (mostly) from scratch. The repo aims to provide all the resources needed to build your very own if you so wished.

There is also the [planted-pro companion app](https://github.com/edwardmiller123/planted-pro-app)! Special thanks to [my wife](https://github.com/LucyRMCodes) for helping with this.

## Firmware
### Requirements
- [ARM GNU Toolchain](https://developer.arm.com/downloads/-/gnu-rm) - download the correct one for your system
- make
	- If on windows you can use a windows version like [Gnuwin32](https://gnuwin32.sourceforge.net/packages/make.htm)

- A device with the STLINK firmware. (For flashing main board) This could be an [official STlink debugger](https://uk.rs-online.com/web/p/debuggers-in-circuit-emulators/1827791) or a [nucleo board](https://www.st.com/en/evaluation-tools/nucleo-f446re.html)
- [OpenOCD](https://openocd.org/pages/getting-openocd.html) (For flashing main board)
	- [STLINK Tools](https://github.com/stlink-org/stlink) - Optional alternative
- gdb-multiarch (for flashing and debugging)


### Building
Compile the firmware with,
```
make fw
```

By default the fw is compiled without any log lines and with the debug LED on the main board disabled. To enable these simply run,
```
make fw LOGLEVEL={log level} (-B)
```
where {log level} ranges from 0 to 4 with 0 being FATAL and 4 being DEBUG. You may need to force a full rebuild with `-B` for make to pick up the symbol changes.

### Flashing The Board

## Main board
### Requirements
- [Kicad](https://www.kicad.org/)
- make
	- If on windows you can use a windows version like [Gnuwin32](https://gnuwin32.sourceforge.net/packages/make.htm)

### Exporting Fabrication Files
The gerber and drill files for the main board can be exported from kicad by running,
```
make pcb
```
This will create a `zip` of all the nessecary files ready to be sent to your favourite PCB manufacturer.

### Schematic
You can export a `pdf` of the schematic with,
```
make schematic
```

## Case
The case was designed in Tinkercad. STL files can be found in the `case` directory but you can also play with the original project online [here](https://www.tinkercad.com/things/aGYA4L6WpCP-planted-pro-v3).
Included in the `case` directory is a project file for [Bambu Studio](https://bambulab.com/en/download/studio) as I was printing on a [Bambu X1 Carbon](https://uk.store.bambulab.com/collections/x1-series) at [work](https://www.codethink.co.uk/). If using a different printer youll need to do the slicing yourself or you can send the `stl` files off to your favourite printing house.

## Constructing

### Requirements
- See previous sections for software requirements

You can generate all the nessecary files for contruction all at once by running,
```
make all
```

A full parts list can be found [here](docs/parts_list.md) in the docs folder along with data sheets for the MCU/ICs on the board and any external modules. 

# TODO