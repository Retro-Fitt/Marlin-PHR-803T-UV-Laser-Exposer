# Marlin Based PHR-803T UV Laser Exposer For PCB Lithography
This project is work in progress so please be advised.

TLDR; This project hardware is based on Diyouware PCB-MKI and software is modified Marlin ver: 1.1.9.1. Project is developed for ease of use because nowadays nobody uses HPGL/1 anymore and Eagle CAD is end of support. I like and use Kicad so G-Code implementation is better for software compatibility i guess.

# Printer mechanical assembly
Mechanical assembly is roughly same as original project. Some 3D parts are modified slightly and all files are redrawn in Autodesk Inventor for better fitment and easy exporting.All design files are in my another repository "here".

I didn't want to use TwinTeeth mechanical design because it is a delta robot and too complex and also i don't need additional features such as drilling, solder paste etc. Another thing that bothered me you have to apply UV sun film everytime which in MK-I is sticked only once and you can use it practically forever. 

# Printer electronic assembly
The hardware is same as "DiyouPDriverV1.0" and "DiyouPDriverV1.0". Original project uses Arduino UNO but since most of UNO shields are compatible with MEGA it is suitable. Another custom shield may be designed for MEGA but i already printed and assembled shield for UNO.

The main disadvantage using shield for UNO is you cannot install a LCD easily.I had to solder 15 pins manually to LCD and SD card controller.RAMPS 1.4 Shield is maybe easier but too much hassle to connect RAMPS 1.4 to Laser Driver. So i have this custom connection to LCD and SD card and modified pins_RAMPS.H accordingly.
# Disclaimer
TODO
## Credits
Marlin Firmware (https://github.com/MarlinFirmware/Marlin)

Diyouware PCB MK-I (http://www.diyouware.com/DiyoPCB-MKI)

Diyouware TwinTeeth (http://diyouware.com/twth_getit)

