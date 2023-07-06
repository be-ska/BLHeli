# BLHeli

## Align M460
This branch add the support for Align M460 ESC, adding the following feature:
- LED control in the main code by digital input
- DIP switch check in the startup code

### DIP switch setting
- DIP 1 ON: white LED ON. if digital input connected can be controlled;
- DIP 1 OFF: white LED always off;
- DIP 2 ON: red LED ON. if digital input connected can be controlled;
- DIP 2 OFF: red LED always off;
- DIP 3 ON: Motor rotation CW (changed);
- DIP 3 OFF: motor rotation CCW (changed).

### Build code
1. Setup your environment downloading this tool (login and download [here](http://support.raisonance.com/products-list)):
    - Raisonance Ride7
    - Rkit-51 toolchain.
1. Main code is at `/SiLabs/BLHeli.asm``
1. Hardware specification at `/SiLabs/Align_M460.inc`
1. To build only for Align M460 just execute `/SiLabs/MakeHexfiles.bat`
1. Default M460 firmware under `/SiLabs/Hex files Align/`

### Flash ESC
To flash:
1. Download [BLHeli suite](https://www.mediafire.com/folder/dx6kfaasyo24l/BLHeliSuite ) file `BLHeliSuite16714903`.
1. Launch `BLHeliSuite.exe`.
1. Plug in your Arduino, a new COM port should appear. Pick the Arduino COM port in BLHeliSuite.
1. Go to the “Make Interfaces” tab, select your Arduino board and click on “Arduino 4way-interface” option.
    - for Arduino Nano choose the one that contains “PD3PD2” and follow this connection: 
        - GND -> GND
        - D2 -> C2D 
        - D3 -> C2CK
    - Arduino Uno or general choose the one that contains “PB3PB4” and follow this connection::
        - GND -> GND
        - D12 -> C2D
        - D11 -> C2CK
1. Connect the ESC following the attached schematic.
1. In BLHeliSuite, select “SILAB C2 (4way-if)” under SILABS Interface.
1. Plug in the battery to power the ESC.
1. Go to "Silabs ESC setup" and click "Flash others".


## BLheli defaul README

This tree contains BLHeli code for sensorless brushless motor electronic speed control (ESC) boards.  
  
To view and use the files, click the "Clone or download" button on this page,  
and then select "Download ZIP" to download the repository to your computer.  
  
For flashing and configuration, download the BLHeliSuite PC software:  
https://www.mediafire.com/folder/dx6kfaasyo24l/BLHeliSuite  
  
For more information, check out these threads:  
https://www.rcgroups.com/forums/showthread.php?2640796 (for BLHeli_S)  
http://www.rcgroups.com/forums/showthread.php?t=2136895 (for BLHeli)  
  
And look in the "BLHeli_32 ARM" folder for info on BLHeli_32.

October 2018,
Steffen Skaug
