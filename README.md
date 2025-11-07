# planted-pro
This is the planted-pro plant monitor. It is my first electronics project and the goal was to create a full product end to end (mostly) from scratch. The repo aims to do provide all the resources needed to build your very own if you so wished

There is also the [planted-pro companion app](https://github.com/edwardmiller123/planted-pro-app)! Special thanks to [my wife](https://github.com/LucyRMCodes) for helping with this.

## Firmware
### Requirements
- [ARM GNU Toolchain](https://developer.arm.com/downloads/-/gnu-rm) - download the correct one for your system
- make
	- If on windows you can use a windows version like [Gnuwin32](https://gnuwin32.sourceforge.net/packages/make.htm)
- For flashing:
	- A device with the STLINK firmware. This could be an [official STlink debugger](https://uk.rs-online.com/web/p/debuggers-in-circuit-emulators/1827791) or a [nucleo board](https://www.st.com/en/evaluation-tools/nucleo-f446re.html)
	- [OpenOCD](https://openocd.org/pages/getting-openocd.html)
		- [STLINK Tools](https://github.com/stlink-org/stlink) - Optional alternative



### Building
Compile the firmware with,
```
make fw
```

## Main board
### Requirements
- [Kicad](https://www.kicad.org/)

## Case
The case was designed in Tinkercad. STL files can be found in the `case` directory but you can also play with the original project online [here](https://www.tinkercad.com/things/aGYA4L6WpCP-planted-pro-v3)

# TODO