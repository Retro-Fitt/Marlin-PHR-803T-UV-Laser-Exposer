# Marlin Based PHR-803T UV Laser Exposer For PCB Lithography
This project is work in progress so please be advised.

TLDR; This project hardware is based on Diyouware PCB-MKI and software is modified Marlin ver: 1.1.9.1. Project is developed for ease of use because nowadays nobody uses HPGL/1 anymore and Eagle CAD is end of support. I like and use Kicad so G-Code implementation is better for software compatibility so i modernized it.

# How original project works

Let me explain how this PCB printer works briefly:

It's just a cartesian 2 axis X-Y printer on top of PHR-803T blueray module.(X-BOX 360 Laser Module)
The blueray module focuses on home position with lens servos and keeps lens position at focus.
The printer which have arduino UNO based motherboard takes HPGL coordinate values and translates it into stepper drivers which in my case TMC2208.
Laser diode exposes tracks-vias etc. into dryfilm, then you develop PCB.(See Dry-Film Parameters)



# Issues I get with orijinal project

The original project was working fine until i get some issues with HPGL/1 code generation.

Since it is quite an old project (2014 dated?) it was designed to use with EAGLE CAD software with HPGL/1 export.I use KiCAD in my projects which exports HPGL/2(which i cannot use with my printer), Gerber,DXF etc.So i have to made and DXF export in KiCAD open it with AutoCAD Plot it with HPGL/1 manually edit code to printers parameter and only in that case i can use it, but since it is a conversion issue, AutoCAD doesn't export HPGL/1 like EAGLE does.Here is some pictures so you can understand what i mean:

Also in small segments such as numbers it overoxposes dry film:

The AutoCAD exports HPGL/1 differently (Diagonal lines on horizontal, Overlapping lines on complex objects such as text and numbers) and my PCB come out as overexposed or underexposed. I have tried many programs to convert DXF, Gerber, postscript such as PloView and GerbView none of them work for me to correctly translate into HPGL/1.Also used different laser power levels and tried many many options but it made me crazy.

As a result i decided to use MARLIN with Gcode and in this repo i will try to implement PHR-803T control in it.
The main problem is to implement auto-focusing algoritm.It uses so called astigmatic method the built in photodiode array UV light and calculuates whether is laser focused or not.(FE Signal)Please see diyouware page if you need more information.

Projects original code is mixed up GRBL, MARLIN and hp2xx.The firmware is very crippled to fit in an Arduino UNO.I have used MEGA instead of UNO because it has more memory, flash storage and i wanted to be independent on pc with print on SD card and LCD controller. Ancient processing code also doesn't help much and freezes time to time.

# Printer mechanical assembly
Mechanical assembly is roughly same as original project. Some 3D parts are modified slightly and all files are redrawn in Autodesk Inventor for better fitment and easy exporting. All design files are in my another repository "here".You can print yourself if you want.Here are some photos of it.

I didn't want to use TwinTeeth mechanical design because it is a delta robot and too complex and also i don't need additional features such as drilling, solder paste etc. Another thing that bothered me you have to apply UV sun film everytime which in MK-I is sticked only once and you can use it practically forever. 

# Printer electronic assembly
The hardware is same as "DiyouPDriverV1.0" and "DiyouPDriverV1.0". Original project uses Arduino UNO but since most of UNO shields are compatible with MEGA it is suitable. Another custom shield may be designed for MEGA but i already printed and assembled shield for UNO.

The main disadvantage using shield for UNO is you cannot install a LCD easily.I had to solder 15 pins manually to LCD and SD card controller.RAMPS 1.4 Shield is maybe easier but too much hassle to connect RAMPS 1.4 to Laser Driver. So i have this custom connection to LCD and SD card and modified pins_RAMPS.H accordingly.

# Dry-Film Parameters

Below parameters are suggested by my dry-film manufacturer, your mileage may vary.

Lamination Parameters:

Manual Laminator
Roll Temperature: 110-130°C (230-266°F)
Roll Speed: 1.0-3.0 mm/min (3-10 feet/min)
Air Assist Pressure: 35-50 psi
Exit Temperature: 40-60°C (104-140°F)

Developing (First Etching) Parameters:

Developer Solution:         Optimum Value Range        Range
Sodium Carbonate:           %0.85                      %0.7-1.0
Potassium Carbonate:        %1.0                       %0.8-1.2
Temperature:                23-30(74-86)               25-35(77-95)

Stripping (For cleaning unexposed dry-film) Parameters:

Temperature:                40-60°C (104-140°F)
Pressure:                   1.0-3.0kg/cm2 (15-45psi)
Concentration:              %2-5 NaOH Solution

Do not expose dry film to sun light, use dry film only in yellow light.
Keep in cold place, no moisture or excessive temperatures.
Lamination, Developing and stripping processes are temperature critical.

I use Potassium Carbonate to etch and NaOH for stripping PCB.For final dissolving i prefer FeCl3.02

# Disclaimer
TODO
## Credits
Marlin Firmware (https://github.com/MarlinFirmware/Marlin)

Diyouware PCB MK-I (http://www.diyouware.com/DiyoPCB-MKI)

Diyouware TwinTeeth (http://diyouware.com/twth_getit)

