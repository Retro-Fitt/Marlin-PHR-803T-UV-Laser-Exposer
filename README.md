# Marlin Based PHR-803T UV Laser Exposer For PCB Lithography
This project is work in progress so please be advised.

**TLDR**; This project hardware is based on Diyouware PCB-MKI and software is modified Marlin ver: 1.1.9.1. Project is developed for ease of use because nowadays nobody uses HPGL/1 anymore and Eagle CAD is end of support. I like and use Kicad so G-Code implementation is better for software compatibility so i modernized it.

## How original project works

Let me explain how this PCB printer works briefly:

* It's just a cartesian 2 axis X-Y printer on top of PHR-803T blueray module.(X-BOX 360 Laser Module)
* The blueray module focuses on home position with lens servos and keeps lens position at focus.
* The printer which have arduino UNO based motherboard takes HPGL coordinate values and translates it into stepper drivers which in my case TMC2208.
* Laser diode exposes tracks-vias etc. into dryfilm, then you develop PCB.(See Dry-Film Parameters)



## Issues With Original Design

The original design was working fine until i get some issues with HPGL/1 code generation.

Since it is quite an old project (2014 dated?) it was designed to use with EAGLE CAD software with HPGL/1 export.I use KiCAD in my projects which exports HPGL/2(which i cannot use with my printer), Gerber,DXF etc.So i have to made and DXF export in KiCAD open it with AutoCAD Plot it with HPGL/1 manually edit code to printers parameter and only in that case i can use it, but since it is a conversion issue, AutoCAD doesn't export HPGL/1 like EAGLE does.Here is some pictures so you can understand what i mean:

![1](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/54075124-6101-4d57-805a-f69586c75d06)


Also in small segments such as numbers it overoxposes dry film:

![2](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/7c11c516-0669-4809-9e1d-7f629971bd88)
![3](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/77758d1e-8b81-4837-9713-8086d0e6d170)

The AutoCAD exports HPGL/1 differently (Diagonal lines on horizontal, Overlapping lines on complex objects such as text and numbers) and my PCB come out as overexposed or underexposed. I have tried many programs to convert DXF, Gerber, postscript such as PloView and GerbView none of them work for me to correctly translate into HPGL/1.Also used different laser power levels and tried many many options but it made me crazy.

As a result i decided to use MARLIN with Gcode and in this repo i will try to implement PHR-803T control in it.
The main problem is to implement auto-focusing algoritm.It uses so called astigmatic method the built in photodiode array UV light and calculuates whether is laser focused or not.(FE Signal)Please see diyouware page if you need more information.

Projects original code is mixed up GRBL, MARLIN and hp2xx.The firmware is very crippled to fit in an Arduino UNO.I have used MEGA instead of UNO because it has more memory, flash storage and i wanted to be independent on pc with print on SD card and LCD controller. Ancient processing code also doesn't help much and freezes time to time.

## Printer mechanical assembly
Mechanical assembly is roughly same as original project. Some 3D parts are modified slightly and all files are redrawn in Autodesk Inventor for better fitment and easy exporting. All design files are in my another repository "here".You can print yourself if you want.Here are some photos of it:


I didn't want to use TwinTeeth mechanical design because it is a delta robot and too complex and also i don't need additional features such as drilling, solder paste etc. Another thing that bothered me you have to apply UV sun film everytime which in MK-I is sticked only once and you can use it practically forever. 

## Printer electronic assembly
The hardware is same as "DiyouPDriverV1.0" and "DiyouPShieldV1.0". Original project uses Arduino UNO but since most of UNO shields are compatible with MEGA it is suitable. Another custom shield may be designed for MEGA but i already printed and assembled shield and driver for UNO.

Assembled DiyouPDriverV1.0:
![Driver](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/ce2b91ec-410a-420e-94ab-1e3c3031c672)


Assembled DiyouPShieldV1.0:
![Shield](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/8767e4c8-8d2b-4cc0-bf65-42de6b71f542)

I translated Eagle PCB Design files to Kicad 7.0 you can find in my another repo:

![Kicad Shield](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/2de5714f-968f-41d8-922c-51229d4563ed)
![Kicad_3D Shield](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/5f1855b1-e959-4634-b4e1-a35498e02cea)


![Kicad Driver](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/be89a58e-2d5d-481a-b763-45c61ac8a69a)
![Kicad 3D Driver](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/6bbae089-651e-4d78-ac5f-94aa0049ab59)

TODO

The main disadvantage using shield for UNO is you cannot install a LCD easily.I had to solder 15 pins manually to LCD and SD card controller.RAMPS 1.4 Shield is maybe easier but too much hassle to connect RAMPS 1.4 to Laser Driver. So i have this custom connection to LCD and SD card and modified pins_RAMPS.H accordingly.

Here is pinout for LCD and SD Card interface:

![Pinout_For_Lcd_SD](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/a6570e7f-58ea-41c7-9a5e-ee35808535dc)

RepRap Ramps 1.4 Compatible 128x64 Full Graphic Smart LCD Interface to Arduino MEGA 

![lcd](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/18ac8f2d-80fa-4526-994b-706ca84d29a6)

And soldered board for EXP1 and EXP2:

TO BE UPLOADED

## Dry-Film Parameters

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

# TO DO LIST:

Not working for now:

* Auto-Focus Code M1500 Custom Code :rage4:
* Custom Timers for PWM⏲️
* Home Screen Icons and Bootscreen🏠
* Documentation 📖

  
Working And Tested:

* Laser on/off, code is based of M3/M5 🔦
* Custom Menu Enabled and Laser Power Levels implemented 🎚️
* Homing X-Y works
* Arduino Pins Map
* LCD interface
* SD Card interface
* Buzzer Interface

## Disclaimer

Lasers are dangerous and not a toy.This project comes WITHOUT ANY KIND OF WARRANTY. You are responsible for your actions.

## Credits

[Marlin Firmware] (https://github.com/MarlinFirmware/Marlin)

[Diyouware PCB MK-I] (http://www.diyouware.com/DiyoPCB-MKI)

[Diyouware TwinTeeth] (http://diyouware.com/twth_getit)

