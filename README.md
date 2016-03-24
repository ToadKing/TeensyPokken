# TeensyPokken
Use a SNES controller in Pokken Tournament, with the help of a Teensy

## Requriements
* A Teensy 3.1/3.2 (tested on 3.2, may also work on 3.0 but YMMV)
* Arduino + Teensyduino (tested on 1.6.8 and 1.28 Beta #1 respectively)
* [SNESpaduino](https://github.com/TacticalCode/SNESpaduino)

## Instructions
* Install the [SNESpaduino](https://github.com/TacticalCode/SNESpaduino) Arduino library (you can download the repo zip file and
  install that)
* Copy the files in `patched-files` over the installed files in `<ARDUINO_INSTALL_DIR>/hardware/teensy/avr/cores/teensy3`
* Copy the `boards.txt` file in the repo over the one in `<ARDUINO_INSTALL_DIR>/hardware/teensy/avr`
* Open the project, set the device type to "Teensy 3.1/3.2" and the USB type to "POKKEN"
* Edit the `CLOCK`, `LATCH`, and `DATA` constants to point to the pins you wired up your SNES controller to

Every button on the SNES controller is mapped to its equivalent button on the Hori Pokken controller, with the exception of ZL
and ZR being missing.

Note that the Pokken software has a different VID/PID than the Teensy normally does, so if you want to reflash it you must hit
the reset button on the Teensy to put it back into the bootloader.
