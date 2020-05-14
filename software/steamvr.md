---
description: >-
  This page will walk you through installing and running the steamVR driver. It
  is currently supported on windows, with linux support in progress. A
  Launcher/Installer is also in progress.
---

# SteamVR

This [steamVR driver](https://github.com/fuag15/project_northstar_openvr_driver) is still a work in progress, if you run into any issues, please reach out on [Discord](https://discord.gg/9TtZhb4). 

#### Working with some polish needed

- Head Tracking  
- Hand Tracking  
- View Projection  
- Skeletal tracking  
- Basic input  
- T265 Sensor integration

#### Notable unfinished parts

- Gesture recognizer

## Prerequisites

Versions of vendored libraries not included and where to get them:

You will need to install the leap motion multi-device drivers in order for this driver to work.  
[- LeapDeveloperKit 4.0.0+52238](https://github.com/leapmotion/UnityModules/blob/feat-multi-device/Multidevice%20Service/LeapDeveloperKit_4.0.0%2B52238_win.zip)

If using the structure core you will need the CrossPlatform SDK 0.7.1 and the Perception Engine 0.7.1  
[https://structure.io/](https://structure.io)

- CrossPlatform SDK 0.7.1

- PerceptionEngine 0.7.1

## Generating Project Files

_**All commands below are run in windows command prompt**_

In the folder in which you want the repo to exist, run the following commands:

```text
git clone git@github.com:fuag15/project_northstar_openvr_driver.git
```

```text
cd project_northstar_openvr_driver
```

```text
mkdir build
```

```text
cd build
```

```text
cmake -G "Visual Studio 16 2019" -A x64 ..
```

## Building the driver in Visual Studio 2019

- Open the generated solution and set northstar to the startup project \(right click the project and choose the set as startup where the gear icon is\) and build. 

**Make sure to target x64 and a Release build to remove any object creation slowness.**

- The release will be in ```build/Release/```and will be comprised of dll files.

- Copy all the dll's to wherever you want to install from, they should be combined into the ```resources/northstar/bin/win64``` directory, make this if it does not exist and put all generated dll's inside.

## Initializing the Driver in SteamVR

- Next register the driver with steamVR \(using **vrpathreg** tool found in SteamVR bin folder\). This is located at 

`C:\Program Files (x86)\Steam\steamapps\common\SteamVR\bin\win64\vrpathreg.exe`

**vrpathreg is a command line tool, you have to run it via the command prompt. To do this, follow these steps.** 

        1\) open command prompt

        2\) run `cd C:\Program Files (x86)\Steam\steamapps\common\SteamVR\bin\win64\`

        3\) run `vrpathreg adddriver <full_path_to>/resources/northstar`

        4\) you can verify the driver has been added by typing `vrpathreg` in command prompt, it will show you a list of drivers installed. 

- at this point vrpathreg should show it registered under "external drivers", to disable it you can either disable in StamVR developer options under startup and manage addons, or by using `vrpathreg removedriver <full_path_to>/resources/northstar`

- Running steamvr with the northstar connected \(and no other hmd's connected\) should work at this point but probably not have the right configuration for your hmd. Take the .json file from calibrating your nothstar and convert it to the format found in`resources/northstar/resources/settings/default.vrsettings`

- restart steamvr. If driver is taking a long time to start, chances are it got built in debug mode, Release mode optimizes a way a bunch of intermediate object creation during the lens distortion solve that happens at startup. If things are still going south please issue a bug report here: 

- if you wish to remove controller emulation, disable the leap driver in SteamVR developer settings.  


