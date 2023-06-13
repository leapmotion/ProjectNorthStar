---
description: Welcome to the next generation of open source spatial computing.
---

# ✨ Introduction

{% hint style="info" %}
You can get next generation Northstar kits from Combine Reality: [https://shop.combinereality.com](https://shop.combinereality.com).
{% endhint %}

<div data-full-width="true">

<figure><img src="../.gitbook/assets/northstar.png" alt=""><figcaption></figcaption></figure>

</div>

Northstar Next is a new Single-cable (USB Type-C) Project North Star variant kit, cost-optimized with an example of lightweight and modular headset design. The use of a single cable and its modularity are the biggest differences with the previous version. The individual parts are as follows:&#x20;

## What's New?

### Our new Northstar Combiner!

Thinner, lighter, more easily mountable! Still has the massive FoV of Leap Motion’s original optics design.

### **Display Driver Board**

* USB-C with DisplayPort Alt-Mode (Not all USB-C host ports support DisplayPort; please make sure in advance that your intended host device has at least one that does.)
* Using same 3.5” 1440 x 1600 VS035ZSM-NW0-69P0 displays from BOE (Grade B\*)
* LT7911D display controller (good relationship with manufacturer, good support, and a stable supply)
* The D variant of the LT7911 series has two MIPI outputs, while together the displays have four. We’re pushing the limit of what a single 4-channel MIPI port can handle, with the benefit of using 25-pin ribbon cables with only half the pins of their predecessors.
* Low-cost alternative to original board, but with 85 – 90Hz maximum stable refresh rate
* Support for audio output
* USB 2 (High-Speed) passthrough to downstream hub or direct peripheral connection
* Backlight brightness control (can be switched off entirely) with non-volatile settings. When you plug in headset, it has the same brightness as when you unplugged it. You can use a microcontroller to adjust the brightness, but you don’t need one connected for the display and backlight circuitry to work.
* 8-pin ribbon connector for future I2S stereo audio output breakout board

### **USB Hub**

* &#x20;Connects to display driver board and doesn’t require any additional external cables
* &#x20;4-port USB 2 High Speed Hub with FE1.1 controller IC.
* &#x20;1A integrated fuse with 1.5A cutoff
* &#x20;3x 6-pin ribbon connectors for USB 2
* 1x 12-pin ribbon connector for USB 2 with passthrough connection to display driver board’s backlight brightness control pins and one GPIO pin connection for switching power to one of the 6-pin ports (for resetting any finicky sensors with enumeration issues like the T261.)

## Modularity

### **Electronics Modularity**

* Driver board: Breakout Audio ports and Brightness controlling pins
* Adapters, the kit includes several USB adapters so that you can connect it to a variety of different sensors that are in use in the North Star community:\
  \- USB-C male connector supporting USB 2 High Speed, with both 6-pin and 12-pin ribbon connectors for direct connection to the Display Driver board or to one of the 6-pin connectors on the PNS Hub board.\
  \- MicroUSB male connector with upstream 6-pin connector; similar to and compatible with the MicroUSB adapter included with our older kits. Works with PNS Hub board.\
  \- Ultraleap SIR170 / Rigel adapter with upstream 6-pin connector for use with our PNS Hub board.\
  \- T261 USB2 High Speed adapter with 6-pin connector. (Note, with USB2 bandwidth, you won’t be able to stream video from the T261 when used concurrently with an Ultraleap or Leap Motion hand-tracking module. You can, however, get a low-latency stream of the 6DOF tracking data generated onboard the device.) The Realsense T261 has been discontinued by Intel, but can still be found. We include this adapter mainly for the convenience of those North Star builders who already have this module.
* Planned microcontroller board (to be sold separately):\
  \- Brightness control\
  \- Eye-tracking (Experimental and in development)\
  \- Additional sensors (like 3DOF with IMU or ambient light sensor)\
  \- User interface (like configuration buttons)\
  \- Compatibility with planned magnetically-coupled module connectors\
  \- Mechanical Modularity\
  \- Easily removable and swappable core electronics bay and sensor pods\
  \- Snap-together / snap-apart design

&#x20;

### **Mechanical Modularity**

* Easily removable and swappable core electronics bay and sensor pods
* Snap-together / snap-apart design

## Main parts specifications:

* Input Interface: USB-C DisplayPort Alt-Mode (requires host port compatibility; a full-featured USB-C 3.2, Thunderbolt 3 or better, or spec-compliant USB 4 are all safe bets)
* Displaying Resolution: 2880x1600 pixels (2x 1600x1440 Displays)
* Frame Rate: 85-90Hz
* Support Brightness control by adjusting digital potentiometer ([Microchip MCP4021T-503E/MC](https://ww1.microchip.com/downloads/en/DeviceDoc/20001945F.pdf))
* Driver board Board dimensions:&#x20;
* USB hub speed: Max 20-30MB/s
* Max useable USB2.0 high speed downstream ports: 4
* 4 Downstream ports, 3 with 6P Connector, 1 port with power switch controlled via the 4th port(12 Pin Connector)
* USB Hub board dimension:&#x20;
