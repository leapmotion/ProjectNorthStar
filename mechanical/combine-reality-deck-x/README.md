---
description: This is the homepage for the Combine Reality Deck-X variant of Northstar.
---

# Combine Reality Deck X

![](../../.gitbook/assets/unknown-4.png)

The Combine Reality Deck X is a variant of Release 3 designed by Noah Zerkin's team at smart-prototyping. It includes a new hub called "The Integrator" which includes microSD card storage, an Arduino and USB hub, an embedded Intel Realsense t261 sensor, and a control board for adjusting ergonomics like IPD and eye relief.&#x20;

{% hint style="info" %}
****[**GitHub Repository**](https://github.com/CombineReality/Deck-X/)
{% endhint %}

## 3D Printing Variants

Please note that the Github repo for the CombineReality Deck X headset has three versions. \
[**Deck X - with Heat Set Inserts**](https://github.com/CombineReality/Deck-X/tree/main/Deck\_X/STL\_files/)\
&#x20;The Prints in the Deck X folder are intended for users who will be assembling, hacking and taking apart their headset multiple times. The inserts are intended to help increase the lifespan of the 3D printed parts by reducing the stress and wear on the parts themselves. This version is not updated as frequently as the version below without heat set inserts.\
[**Deck X - without Heat Set Inserts**](https://github.com/CombineReality/Deck-X/tree/main/Deck\_X/STL\_files/)\
There's also a version of the Deck X for users that don't want to use heat-set inserts. Please note that if you only intend to adjust or rebuild the headset once or twice maximum. Taking the headset apart and putting it back together without heat-set inserts will cause the mounting points to deteriorate over time. \
[**3.1.1**](https://github.com/CombineReality/ProjectNorthStar/tree/master/Mechanical/CombineReality\_Variants/3.1.1)\
The Prints in the 3.1.1 folder are intended for users who want to upgrade their existing 3.1 headset without reprinting the optics bracket.

## Integrator Board

The Integrator is our custom-built USB hub system originally created for the CombineReality Project North Star Deck X. The Integrator cuts down the use of cables and adds customizable buttons to the headset with the following components & features:

* USB-C hub, two USB 3.1 ribbon connectors, and one USB 2.0 ribbon connector 3GB on-board flash drive (only works when connected to a USB 3.0 host) Arduino-compatible microcontroller, featuring a Qwiic connector that can be used to connect additional sensors like an IMU, as well as HID buttons that can emulate keyboard keys. \

* A button breakout board is included, and the microcontroller is preflashed with firmware that maps the buttons to the default ergonomics adjustment keys. (Eye relief, eye position, and IPD) Also allows for manual power reset of sensor USB ports via a GPIO pin. \

* A fan, the speed of which is controlled by the Arduino-compatible microcontroller. A thermistor for a more intelligent fan speed control.\

* &#x20;A ribbon connector that lets the Arduino on the hub relay commands and debug output to and from the serial UART of the display driver board Ribbon adapter board for Intel® RealSense™ T261 embedded 6-DOF module (ribbon cable included) Ribbon adapter board for Leap Motion Controller (ribbon cable included)

The Integrator uses a modified version of the lilyPadUSB-caterina Arduino bootloader Bootloader can be found here: [https://github.com/CombineReality/ArduinoCore-avr](https://github.com/CombineReality/ArduinoCore-avr)

![](<../../.gitbook/assets/image (25).png>)

![](../../.gitbook/assets/arduino\_pins.png)

{% content-ref url="deck-x-assembly-guide.md" %}
[deck-x-assembly-guide.md](deck-x-assembly-guide.md)
{% endcontent-ref %}

{% content-ref url="deck-x-bill-of-materials.md" %}
[deck-x-bill-of-materials.md](deck-x-bill-of-materials.md)
{% endcontent-ref %}

{% content-ref url="../headgear-assembly.md" %}
[headgear-assembly.md](../headgear-assembly.md)
{% endcontent-ref %}





