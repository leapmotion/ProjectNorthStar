---
description: >-
  This is the procedure to install the proper software to build the AR Display
  Board firmware and the procedure to install the firmware on to the device.
---

# Project North Star AR Display Board

## Set Up

The first thing that you will need to do is to install Atmel Studio. Atmel is now owned by Microchip, so you will need to go to the following web page to download and install Atmel Studio for your version of Windows:

{% embed url="https://www.microchip.com/mplab/avr-support/atmel-studio-7" caption="" %}

The next step is to clone the Project North Start repository from github. So let's first create a directory in which we can install all of our development source code. So start by bringing up a windows terminal and then enter the following commands:

```text
C:\>mkdir Development
C:\>cd Development
C:\Development>git clone https://github.com/leapmotion/ProjectNorthStar.git
```

## Building the firmware

You will need to open Atmel Studio and select the following solution file for the AR Display Board:

```text
C:\Development\ProjectNorthStarElectronics\Firmware\atmel_project.atsln
```

Then to build the firmware, it's just a simple matter to select Build Solution from the Build menu.

## Installing the firmware

The first thing that you will need to do is to connect the display board to the Atmel SAM programmer and then connect the programmer to your computer using a USB cable. Once connected, return to Atmel Studio and open Device Programming from the Tools menu. In the Device Programming window, the programmer should show up in the Tool/Device/Interface display fields. When you click on the Apply button, the lower windows will be filled in. Now click on the Memories text in the left window and this will bring up the fields to program the different types of memory in the device. Make sure the Flash field lists the \*.elf file to be loaded and then press the Program button. If all goes well, the lower text window will display the results.

## Command Line Installation

You can use the following command to install the firmware from the command line:

```text
C:\>atprogram --tool atmelice --interface SWD --device ATSAMD21G18A program -f Program.elf
```

