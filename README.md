# Marlin Based PHR-803T UV Laser Exposer For PCB Lithography
**This project is work in progress so please be advised.**

**TLDR**; This project hardware is based on Diyouware PCB-MKI and software is modified Marlin 1.1.9.1. Project is developed for ease of use because nowadays nobody uses HPGL/1 anymore and Eagle CAD is end of support. I like and use Kicad so G-Code implementation is better for software compatibility so i modernized it.

## How original project works

Let me explain how this PCB printer works briefly:

* It's just a cartesian 2 axis X-Y printer on top of PHR-803T blueray module.(X-BOX 360 Laser Module)
* The blueray module focuses on home position with lens servos and keeps lens position at focus.
* The printer which have arduino UNO based motherboard takes HPGL coordinate values and translates it into stepper drivers which in my case TMC2208.
* Laser diode exposes tracks-vias etc. into dryfilm, then you develop PCB.(See Dry-Film Parameters)



## Issues With Original Design

The original design was working fine until i get some issues with HPGL/1 code generation.

Since it is quite an old project (2014 dated?) it was designed to use with EAGLE CAD software with HPGL/1 export. I use KiCAD in my projects which exports HPGL/2(which i cannot use with my printer), Gerber,DXF etc. So i have to made and DXF export in KiCAD open it with AutoCAD Plot it with HPGL/1 manually edit code to printers parameter and then i can use it, but since it is a conversion issue, **AutoCAD doesn't export HPGL/1 like EAGLE does.** EAGLE export is like raster export AutoCAD is like dot printer. Here are some pictures so you can imagine what i mean:

![1](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/2b16004d-4faa-4acb-9769-3c8fe575952f)

Also in small segments such as numbers it overoxposes dry film:

![3](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/d48fd775-ea9d-4c31-8d4d-76278363edcf)
![2](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/69304bcb-427a-47b0-bbef-0e4b76bf2b8a)


The AutoCAD exports HPGL/1 differently (Diagonal lines on horizontal, Overlapping lines on complex objects such as text and numbers) and my PCB come out as overexposed or underexposed. I have tried many programs to convert DXF, Gerber, postscript such as PloView and GerbView none of them work for me to correctly translate into HPGL/1. Also used different laser power levels and tried many many options but it made me crazy. 

As a result i decided to use MARLIN with Gcode and in this repo i will try to implement PHR-803T control in it.
The main problem is to implement auto-focusing algoritm. It uses so called astigmatic method the built in photodiode array UV light and calculuates whether is laser focused or not. (FE Signal)Please see diyouware page if you need more information.

Projects original code is mixed up **GRBL**, **MARLIN** and **hp2xx**.The firmware is very crippled to fit in an Arduino UNO. I have used MEGA instead of UNO because it has more memory, flash storage and i wanted to be independent on pc with print on SD card and LCD controller. Ancient processing code also doesn't help much and freezes time to time.

## Printer mechanical assembly
Mechanical assembly is roughly same as original project. Some 3D parts are modified slightly and **all files are redrawn in Autodesk Inventor** for better fitment and easy exporting. All design files are in my another repository **"here"**. You can print yourself if you want, measuring some details-features are more easier. Here are some renders and photos of it:


![Inventor1](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/7ee933ba-bbc0-4aa6-b9d3-9709046d539e)
![Inventor2](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/8a201964-fc3f-47a1-8179-c302227463c1)
![Real_Printer_Assembly](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/9026fc39-4ed8-4373-9f02-e32638c1d453)



I didn't want to use TwinTeeth mechanical design because it is a delta robot and too complex and also i don't need additional features such as drilling, solder paste etc. Another thing that bothered me you have to apply UV sun film everytime which in MK-I is sticked only once and you can use it practically forever. 

## Printer electronic assembly
The hardware is same as "DiyouPDriverV1.0" and "DiyouPShieldV1.0". Original project uses Arduino UNO but since most of UNO shields are compatible with MEGA it is suitable. Another custom shield may be designed for MEGA but i already printed and assembled shield and driver for UNO.

**Assembled DiyouPDriverV1.0:**

![Driver](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/cc20ee2f-d8e3-483e-a6f9-91fc2f47079e)

**Assembled  DiyouPShieldV1.0:**

![Shield](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/421fd321-06d8-462f-8529-1794fd125a0e)


I translated Eagle PCB Design files to Kicad 7.0 you can find in my **another repo:**

TODO

![Shield3D](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/c68323bf-ea74-4169-910b-779b1bbb3761)
![Shield](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/4f5a3b34-efe1-4cda-b9b8-551a54840fa1)
![Driver3D](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/9bb71832-a451-4ed3-9ad7-be9151eda701)
![Driver](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/d7e0b176-f176-4c11-836b-ac6525846918)

TODO

The main disadvantage using shield for UNO is you cannot install a LCD easily.I had to solder 15 pins manually to LCD and SD card controller.RAMPS 1.4 Shield is maybe easier but too much hassle to connect RAMPS 1.4 to Laser Driver. So i have this custom connection to LCD and SD card and modified pins_RAMPS.H accordingly.

Here is pinout for LCD, SD Card and Shield interface:

![Pinouts](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/7ed14288-38ff-4021-8965-84aedeb375c4)


RepRap Ramps 1.4 Compatible 128x64 Full Graphic Smart LCD Interface to Arduino MEGA 

![lcd](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/c2c03a1d-9910-41b6-b5c4-871f5e59f73c)

And soldered board for EXP1 and EXP2:
![Adapter](https://github.com/Retro-Fitt/Marlin-PHR-803T-UV-Laser-Exposer/assets/5271947/f9ca0438-0225-4977-ad29-850d395d4ed1)

I keeped red PCB adapter for testing with RAMPS 1.4 Board.You can solder directly EXP1 and EXP2 connectors if you wish.

## Dry-Film Parameters

Below parameters are suggested by my dry-film manufacturer, your mileage may vary.

Lamination Parameters:

|Manual Laminator     |                               |
| ------------------- |-------------------------------|
|Roll Temperature:    | 110-130°C (230-266°F)         |
|Roll Speed:          | 1.0-3.0 mm/min (3-10 feet/min)|
|Air Assist Pressure: | 35-50 psi                     |
|Exit Temperature:    | 40-60°C (104-140°F)           |

|Developing (First Etching) Parameters:|                          |                    |
| ------------------------------------ |--------------------------| -------------------|
|Developer Solution:                   |  Optimum Value Range     |   Range            |
|Sodium Carbonate:                     |  %0.85                   |   %0.7-1.0         |
|Potassium Carbonate:                  |  %1.0                    |   %0.8-1.2         |
|Temperature:                          |  23-30°C(74-86°F)        |   25-35°C(77-95°F) |

| Stripping (For cleaning unexposed dry-film) Parameters: |                        |
| ------------------------------------------------------- |------------------------| 
|Temperature:                                             |    40-60°C (104-140°F) |
|Pressure:                                                |1.0-3.0kg/cm2 (15-45psi)|
|Concentration:                                           |   %2-5 NaOH Solution   |

Do not expose dry film to sun light, use dry film only in yellow light. 
Keep in cold place, no moisture or excessive temperatures. 
Lamination, Developing and stripping processes are temperature critical.

I use Potassium Carbonate to etch and NaOH for stripping PCB. For final dissolving i prefer FeCl3.

# TO DO LIST:

Not working or work in progress for now: ⛔

* Home Screen Icons and Bootscreen🏠
* Documentation 📖
* Laser on/off, code is based of M3/M5, custom code is necessary

Working but will checked on production :suspect:

* Auto-Focus works now. M1500 :godmode:
  
Working And Tested: 🆗

* Custom Menu Enabled and Laser Power Levels implemented
* Homing X-Y works
* Arduino Pins Map
* LCD interface
* SD Card interface
* Buzzer Interface
* Custom Timers works and Focus servo is also converted to PWM and TIMER 

## Why i use/make this instead of UV lasers on market❔

* My main attraction is in this project is **resolution**. I mostly deal with high density QFN and QFP packages which may have 0.1mm pin pitch.

* Modified Laser Printer Head assemblies can be used also for lithography. There are people in Youtube "Homemade Laser Exposer" who modified old laser printer, added UV laser to rotating mirror. They seem very fast for non resolution required applications.

## Why i don't use PCB services❓

* I need quick PCB prototypes. Tried many PCB services but in avarage it comes in my door afer 27 days. Even i make one mistake on PCB design i have to wait approx 27 days to test it. So lack of motivation occurs on that project.

* Sometimes, i have an urgent feel of unstoppable PCB making frenzy.

* Where is all the fun?

## Disclaimer ⚠️

Lasers are dangerous and not a toy.This project comes WITHOUT ANY KIND OF WARRANTY. You are responsible for your actions. I am not a software developer so if you find any issue or you have better idea please open up an issue or pull request.

## Credits 🔖

[Marlin Firmware] (https://github.com/MarlinFirmware/Marlin)

[Diyouware PCB MK-I] (http://www.diyouware.com/DiyoPCB-MKI)

[Diyouware TwinTeeth] (http://diyouware.com/twth_getit)

