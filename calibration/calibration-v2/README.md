---
description: This page describes the new method for calibrating optics.
---

# Calibration Setup

## Calibrating Hand Position

Hand position is dependent on the position of the leap motion sensor, make sure your Leap motion sensor has the bottom metal bezel hidden behind the 3d printed housing. You can use this application to setup your hand position: [https://github.com/fmaurer/NorthStarToolbox](https://github.com/fmaurer/NorthStarToolbox)

## Optics Calibration

Due to the nature of 3D printing and assembly each headset is going to be slightly unique and will require going through a calibration process to display the image correctly.   
  
There are currently two ways to calibrate a northstar headset. The first method uses two stereo cameras to calculate the 3D position of the displays and reflectors. The second method uses a single stereo camera, and is currently setup to be able to use the intel t265 camera, which we currently recommend for 6DOF. This allows northstar developers to reuse the t265 rather than purchase two seperate stereo cameras. 

## Calibration V1 and V2 Comparison

| **Features** | **Calibration V1** | **Calibration V2** |
| :--- | :--- | :--- |
| IPD adjustment | Yes | In Progress |
| T265 Support | No | Yes |
| Cameras | 2 \(stereo\) | 1 \(stereo\) |
| Camera Calibration | Checkerboard Process | Built In or Checkerboard |

## Calibration Stand Assembly

Version One and Version Two of the calibration setup share the same 3D printable stand. You can find the assembly instructions below. \(Note that they show the instructions for the original dual camera stand\)

![](../../.gitbook/assets/image%20%281%29.png)

![](../../.gitbook/assets/image%20%282%29.png)

![Camera Holder Assembly for V1](../../.gitbook/assets/image%20%283%29.png)



## Setup Calibration V1

This requires:

* 3D Printing the mechanical assembly, search for it here: [https://leapmotion.github.io/ProjectNorthStar/](https://leapmotion.github.io/ProjectNorthStar/)
* Affixing TWO of these Stereo Cameras to it: [https://www.amazon.com/ELP- Industrial- Application- Synchronized- ELP- 960P2CAM- V90- VC/dp/B078TDLHCP/](https://www.amazon.com/ELP-Industrial-Application-Synchronized-ELP-960P2CAM-V90-VC/dp/B078TDLHCP/)
* Acquiring a large secondary monitor to use as the calibration target
* Find the exact model and active area of the screen for this monitor; we'll need it in 3\)

## Setup Calibration V2

