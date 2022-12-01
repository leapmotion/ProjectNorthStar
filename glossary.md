---
description: A collection of terms and acronyms of varying degrees of ambiguity
---

# Glossary

Given that Northstar is a project that combines many different fields of research and expertise, it's unlikely that any one person will ever know or be able to recall all the terms, acronyms and technical jargon that is used to discuss these various industries. This glossary intends to define some of the more particular terms, starting with the field of electrical engineering, but will eventually grow to cover other topics in more depth, like firmware, software etc. \
\
This page is open to [**pull requests**](glossary.md#pull-request), please feel free to submit PRs with more terms!&#x20;

## General Terms

### XR

XR is an umbrella term for Virtual Reality, Augmented Reality and Mixed Reality. Some say it stands for Extended Reality, others say the X is a variable, check twitter if you want to see people argue about it for literal years. In general, XR is the term the northstar community uses when discussing VR/AR or MR. For more general discussions we tend to enjoy using "Spatial Computing".

### Combiner

A Combiner is an Optical component that takes rays of light and focuses them to a single end point.&#x20;

### 6DOF

So, DOF is a fun one because there are two meanings for the acronym. In traditional camera work it means Depth of Field. For Northstar and other XR devices it means "Degrees of Freedom". In a 6-DOF system you can rotate on 3 Axis and also move on 3-axis.&#x20;

### IPD

IPD stands for Interpupilary distance. This is the term for the distance between your pupils, which is useful information when correcting for optical distortion.

### Eye Box

The Eye Box is the area in which the image from a headset is clearly visible. Northstar has a fairly large Eye Box meaning you can adjust the headset to be more comfortable without worrying too much about whether or not you've got it positioned just right.&#x20;

### FOV

FOV stands for field of view, this is usually referring to how much you can see through a headset, and can also be used to describe the amount a camera can see. FOV can be measured horizontally, vertically or diagonally.

### Latency

Latency is an overall term for how long a signal takes to get from one location to another, this is applicable to many areas of the northstar project. For example there is latency from when an image is rendered by the computer to when it is displayed on the northstar screen, or latency from when the image from a tracking camera makes its way to the onboard processor. In short, latency is time, and the longer something takes the more latency it has.&#x20;

### VAC

VAC stands for the Vergence Accomodation Conflict. This term describes the difficulty our eyes have in focusing on objects that optically appear a certain distance away and via stereo disparity appear a different distance away.

### DP

DP stands for Display Port. This is the display signal type that northstar uses. In general, converting from DP to HDMI is easy, converting from HDMI to DP is incredibly difficult and rarely works, in our experience it is a waste of time trying to look into HDMI to DP adapters.&#x20;

#### DP Alt Mode

DP alt mode is a specific subsection of the USB Type C spec that allows it to carry a display port signal.&#x20;

### PD

PD stands for power delivery. This is another subsection of the USB specification that specifies a USB port has the neccessary components to supply more power than a standard port.&#x20;

## Electrical Engineering Terms 🔋

### MCU

MCU stands for Micro-controller. Microcontrollers are little self-contained computers in a chip that execute programs called firmware. These chips are on PCBs, or printed circuit boards. The programs control various peripherals that are either built into the chip or connected externally. Popular examples of Micro-controllers include [**Arduino**](https://www.arduino.cc) and Rasberry Pi.&#x20;

### MIPI

MIPI is a general industry group and standard for display signal interfaces and camera signal interfaces, among others. For Northstar's case we care mostly about display and camera signals. You can read more about MIPI [**here**](https://www.mipi.org/groups/display).&#x20;

### HID

HID stands for Human Interface Device, it a set of standards used to connecting peripherals like keyboards, mice etc.. intended for 'driverless' operation. You can read about the specification [**here**](https://www.usb.org/hid), in addition Microsoft have a great resource on HID [**here**](https://docs.microsoft.com/en-us/windows-hardware/drivers/hid/).&#x20;

### I2C

**I2C** stands for **Inter-Integrated Circuit,** It is widely used for attaching lower-speed peripheral [ICs](https://en.wikipedia.org/wiki/Integrated\_circuit) to processors and [microcontrollers](glossary.md#mcu) in short-distance, intra-board communication.

### SPI

The **Serial Peripheral Interface** (**SPI**) is a synchronous serial communication interface specification used for short-distance communication, primarily in embedded systems. The interface was developed by Motorola in the mid-1980s and has become a de facto standard. Typical applications include Secure Digital cards and liquid crystal displays.You can learn more about SPI [**here**](https://en.wikipedia.org/wiki/Serial\_Peripheral\_Interface).

### DP to MIPI Bridge

A DP to MIPI bridge is a specific chip designed to convert signal from a DP input to a MIPI signal to be read by displays.&#x20;

## Software Terms 🧑🏽‍💻

### Git

Git is a common type of Source Control, Allowing developers to maintain a history of their code which is incredibly helpful in diagnosing and figuring out undesired results of changes to that code. You can learn more about Git here: [**https://docs.gitlab.com/ee/topics/git/**](https://docs.gitlab.com/ee/topics/git/)****

### Pull Request

A Pull Request is a term used to describe the act of submitting a piece of code for review and merging into the main code base. You can learn more about pull requests [**here**](https://docs.github.com/en/github/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-pull-requests)****

### Runtime&#x20;

A runtime is an intermediary process between end user applications and hardware. In Northstar's case runtimes can include [**SteamVR**](software/steamvr.md), Monado and [**Esky**](software/esky.md)**.**

#### [OpenXR Runtime](https://www.khronos.org/registry/OpenXR/specs/1.0/html/xrspec.html#runtime)

An OpenXR runtime is software which implements the OpenXR API. There **may** be more than one OpenXR runtime installed on a system, but only one runtime can be active at any given time.

### OpenXR

OpenXR is an API (Application Programming Interface) for XR applications. XR refers to a continuum of real-and-virtual combined environments generated by computers through human-machine interaction and is inclusive of the technologies associated with virtual reality (VR), augmented reality (AR) and mixed reality (MR). OpenXR is the interface between an application and a runtime. The runtime may handle such functionality as frame composition, peripheral management, and raw tracking information.

### SLAM&#x20;

SLAM is a term that originated in robotics meaning Simulatenous Localization and Mapping. It's become the generic term for systems that can see their environment, localize themselves to a previously known location, and generate a map of that environment for other devices to localize against.&#x20;

### VIO

VIO stands for Visual Intertial Odometry, VIO is an important part of the processing needed for SLAM, but does not provide localization or mapping. VIO is soley performing the act of combing visual data with inertial measurements from an [**IMU**](glossary.md#imu).&#x20;

## Hardware/Manufacturing Terms 🛠

### IMU&#x20;

IMU stands for Inertial Measurement Unit, it is a device that can read out vectors like acceleration, and gravity.In Northstar's case these devices are used for 3DOF movement and can help with [**VIO**](glossary.md#vio) and [**SLAM**](glossary.md#slam) [**https://en.wikipedia.org/wiki/Inertial\_measurement\_unit**](https://en.wikipedia.org/wiki/Inertial\_measurement\_unit)****

####
