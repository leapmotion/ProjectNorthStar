---
description: >-
  The SteamVR Driver is comprised of a Launcher, Northstar HMD Driver, intel Realsense t265 Driver, and SDraw's LeapDriver. 
---

# SteamVR
The steamvr repo is [**here**](https://github.com/druidsbane/openvr-northstar).
You will need to have [**SteamVR**](Steam/Steamvr##](https://store.steampowered.com/about/) installed in order for the launcher to work properly. 

## Known issue
Currently due to the use of extended mode, tracking doesn't fully work as intended, you can read more about [**here**](https://github.com/ValveSoftware/openvr/issues/1336). Unfortunately this issue is out of our hands as driver devlopers and is and underlying issue with steamvr. 

## Rationale

The driver is currently divided into multiple parts to reflect the modularity of the northstar project. The launcher provides a single place to configure steamvr with the proper settings. It has a field to allow you to input your device specific calibration file, as well as options to enable or disable drivers, which can be useful if you're using a device other than the realsense t265. 

## Modules
*Launcher- allows configuration of driver files and input calibration file.
*Northstar HMD Driver -Handles HMD functions, image distortion. 
*T265 Driver- Handles 6dof tracking via tracking overrides. 
*SDraw's [**LeapDriver**](https://github.com/SDraw/driver_leap), simulates valve index controllers with leap motion input.

## Compatibility

The steamvr launcher currently runs on both windows and linux*. It is unsupported on MacOS due to steamvr's lack of support for the platform. 

*Leapmotion v4 software is only available on windows, there are plans for the next version, v5 to have support for linux, currently to run leap hand tracking on linux you'll have to use v2 software.

## Installing the launcher

We have included a pre-built version of the steamvr driver under [**releases**](https://github.com/druidsbane/openvr-northstar/releases). You will need to install and run steamVR at least once before running the launcher for the first time.

## Prerequisites for building from source

If you wish to build from source, simply follow these steps:
Instsall the following prerequisistes:
* [##Visual studio 2019##](https://visualstudio.microsoft.com/vs/) (be sure to select Desktop development with C++ and .Net Development)
* [##cmake##](https://cmake.org/download/)
* [##Git##](https://git-scm.com/download/win/)

## Building from source on Windows 10

*Launch the windows command prompt
*Run the following:
``git clone --recurse-submodules https://github.com/druidsbane/openvr-northstar.git
cd openvr-northstar
build.bat``

## Building from source on Linux (PopOS/Ubuntu 19.10)

*Install Steam and Steamvr
*Launch SteamVR at least once
*Launch a terminal window
* Run the following
`` git clone --recurse-submodules https://github.com/druidsbane/openvr-northstar.git
cd openvr-northstar
./linux.sh``