---
description: >-
  The world is full of stereo cameras and sensors, this page will help you
  choose which one to get for your headset.
---

# 📸 Sensor and Tracking Guide

The Northstar Next platform is designed to be modular, this means you can swap in or use any tracking camera/platform you'd like! Given how quickly the landscape around tracking changes, and how often new components/parts come out it's important to be able to upgrade electronics instead of having to build a whole new HMD, it's the **reuse** part of reduce/reuse/recycle!&#x20;

Below are a few recommendations for various sensors, they all provide different features and have pros/cons so be sure to ask on discord if you have any specific questions!&#x20;

## Dedicated Hand Tracking Sensors

### [Ultraleap](https://www.ultraleap.com/product/stereo-ir-170/)

Generally when it comes to out of the box hand tracking, Ultraleap (formerly leap motion) are the best in the field. They have a variety of sensors you can choose. \
For Northstar headsets we generally recommend using the SIR170 as it's the lightest available sensor, and is designed for integrating into headsets. However you can also use the 1st or second gen leap motion cameras. The LeapMotion platform currently works on Windows and Linux, with MacOS support coming soon!&#x20;

{% embed url="https://www.ultraleap.com/product/stereo-ir-170/" %}

{% embed url="https://www.ultraleap.com/product/leap-2/" %}

## Position Tracking Sensors

Throught the duration of Project Northstar there have been many 6DOF sensors on the market. In this category, we'll show sensors that can be used "out of the box" with minimal configuration.&#x20;

{% hint style="info" %}
All the sensors below use a combination of Visual [SLAM](../glossary.md#slam) + [IMU](../glossary.md#imu) data.
{% endhint %}

### Xvisio XR50

The Xvisio XR50 sensor is one of the smallest 6DOF tracking sensors available. It uses a mix of onboard and on host compute to calculate pose, and has some other available features like Plane Detection.&#x20;

{% embed url="https://www.xvisiotech.com/product/seersense-xr50/" %}

### Intel Realsense T261/T265

{% hint style="warning" %}
These cameras are no longer being manufactured or supported by Intel, and you'd likely have to find them second hand. \
\
Note that the last version of the Realsense SDK that supports these sensors is[ 2.53.1](https://github.com/IntelRealSense/librealsense/releases/tag/v2.53.1)
{% endhint %}

While these cameras are now in the end of life stage, they do still function and are generally one of the simplest tracking platforms to get started with. The t261 is just the embedded version of the T265, which means it's better suited for use in a weight-sensitive project like this one, but either sensor will function the same. The T26x sensors provide access to a stream of camera poses directly from the device, which means your computer doesn't have to process the poses.&#x20;

### SteamVR Lighthouse solutions

If you want more reliable 6DOF tracking at the cost of portability, you can choose to use a SteamVR Lighthouse based solution. There are a variety of trackers available for this system, the most common being from HTC Vive and Tundra Labs.&#x20;

Lighthouse based solutions use two external laser sources to track the position of the tracker. The Lighthouse sensors themselves can be acquired via Valve or HTC.&#x20;

{% embed url="https://tundra-labs.com/products/additional-tracker?pr_prod_strat=use_description&pr_rec_id=0d9d5793f&pr_rec_pid=6762248863953&pr_ref_pid=7171093102801&pr_seq=uniform" %}

{% embed url="https://www.vive.com/us/accessory/tracker3/" %}

## General Sensors

### [Luxonis](https://www.luxonis.com)

Luxonis cameras are designed for a variety of computer vision uses cases, and can work well for hand tracking, but aren't explicitly designed for the task. \
This won't be an "out of the box" solution for hand tracking like Ultraleap, but\
if you're more of a tinkerer, and willing to set up a lot of the software yourself, you can try a camera from Luxonis and use [**Monado's**](../software/monado.md) open source hand tracking on Linux platforms.&#x20;

The Luxonis cameras also sport an onboard movidus AI chip which means you can run other computer vision tasks as well, directly on the sensor itself.&#x20;

There are many variants of Luxonis cameras to choose from, we generally recommend the Oak-D-W or Oak-D-Pro-W. It is important to get the -W variants, as these are the wide FOV versions.&#x20;

{% embed url="https://shop.luxonis.com/collections/oak-cameras-1/products/oak-d-w?variant=43905772519647" %}

{% embed url="https://shop.luxonis.com/collections/oak-cameras-1/products/oak-d-pro-w" %}
