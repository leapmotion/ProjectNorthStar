---
description: >-
  This page answers some more general northstar related questions. The other
  sections below it answer questions related to their respective topics.
---

# FAQ

{% hint style="success" %}
have a question not mentioned here? Feel free to ask on the community [Discord](https://discord.gg/9TtZhb4)
{% endhint %}

### How can I build it?

Good Question! There are multiple variations of the reference design from leap motion. The reference design can be found on [this page ](../mechanical/release-3/)

### What are the Minimum System Requirements?

The minimum system requirements for Northstar can vary based on the type of application you want to run. _**Your system will need a display port connection that can output 4k@60hz.**_&#x20;

| Minimum Requirements              | Recommended Requirements        |
| --------------------------------- | ------------------------------- |
| AMD Ryzen Quad Core or Equivalent | AMD Ryzen 6 or 8 Core Processor |
| 8GB Ram                           | 16GB Ram                        |
| Vega 8 GPU or MX150               | Nvidia 1060 6GB or higher       |

### How does project Northstar compare to other AR Platforms?

Project Northstar uses the same displays commonly used in VR headsets to provide a high resolution and high field of view experience.&#x20;

#### Hardware Comparison

| Specifications       | Northstar      | Hololens 2          | MagicLeap     | ARKit        | ARCore       |
| -------------------- | -------------- | ------------------- | ------------- | ------------ | ------------ |
| **Operating System** | Windows, Linux | Windows Holographic | Lumin         | iOS          | Android      |
| **Stand Alone**      | Compute Pack\* | Headset             | Compute Pack  | Phone/Mobile | Phone/Mobile |
| **FOV**              | \~110x70       | \~52 Diagonal       | \~40 Diagonal | N/A          | N/A          |

#### Software Comparison

| Functionality           | Northstar | Hololens 2 | MagicLeap | ARKit   | ARCore   |
| ----------------------- | --------- | ---------- | --------- | ------- | -------- |
| **Positional Tracking** | ✅         | ✅          | ✅         | ✅       | ✅        |
| **Hand Tracking**       | ✅         | ✅          | ✅         | ✅       | ❌        |
| **SteamVR**             | 👷‍♂️     | ❌          | ❌         | ❌       | ❌        |
| **Plane Tracking**      | ❌         | ✅          | ✅         | ✅       | ✅        |
| **Anchors**             | ✅         | ✅          | ✅         | ✅       | ✅        |
| **Light Estimation**    | ❌         | ❌          | ❌         | ✅       | ✅        |
| **Environment Probes**  | ❌         | ❌          |           | ✅       | ✅        |
| **Face tracking**       | ❌         | ❌          | ❌         | ✅       | ✅        |
| **Meshing**             | ❌         | ✅          | ✅         | lidar\* | DepthKit |
| **2D image tracking**   | Aruco     | ✅          | ✅         | ✅       | ✅        |
| **Speech Input**        | ❌         | ✅          | ✅         |         |          |

### Where can I get the lenses and electronic components?

While most of the design is 3d printable, there are components, like the screens, driver board, combiners, and leap motion controller that you will have to order. You can order your parts or full kits from the shop here:

{% embed url="https://shop.combinereality.com" %}

### What 3d printer should I get?

Most components fit within a print volume of 130mm\*130mm\*130mm, however the two largest prints will need a print volume of 220mm\*200mm\*120mm. It is possible for the parts to be split, using [MeshMaker ](https://www.youtube.com/watch?v=afLolx2OEKE\&feature=youtu.be\&t=93)to allow them to fit on smaller print volumes. \
\
The ender series (220 x 220 x 250mm) by Creality seems to be a fan favorite among the discord if you're just getting started with 3d printing. If you want something that has a larger print area, check out the creality pro (300 x 300 x 400mm). If you want other recommendations, feel free to ask on the discord.

{% embed url="https://www.creality3d.shop/collections/3d-printer/products/creality-ender-3-3d-printer-economic-ender-diy-kits-with-resume-printing-function-v-slot-prusa-i3-220x220x250mm" %}
Ender3d
{% endembed %}

{% embed url="https://www.creality3d.shop/collections/3d-printer/products/creality-cr-10s-diy-desktop-3d-printer-300x300x400mm" %}
Creality Pro
{% endembed %}

### How can I track my position?

The Intel RealSense T265 is the most commonly used device currently. It supports 6dof (degrees of freedom) but does not support world meshing.&#x20;

~~The occipital structure core is great since it's cross platform and non GPU dependent and has more features, but it's more expensive than the Realsense. (Note that if you order this you need the black and white camera version and NOT the color version). There are members of occipital here in the discord to answer more questions, check out the #occipital-structure-core channel.~~ **Occipital has discontinued support of the perception engine and is no longer recommended.**&#x20;

If you have a windows PC with a 1070 or above you can use the Zed Mini, but it only works with Nvidia CUDA which limits its use.

If you have a Vive already, you can use a vive tracker for 6dof tracking, however the vive tracker requires external "lighthouse" base stations in order to function, making it more difficult to transport the headset or use it in different environments without extra setup.

### What is Calibration?

Due to the nature of 3D printing and assembly each headset is going to be slightly unique and will require going through [a calibration process](../calibration/getting-started/) to display the image correctly. \
\
There are currently two ways to calibrate a northstar headset. The first method uses two stereo cameras to calculate the 3D position of the displays and reflectors. The second method uses a single stereo camera, and is currently setup to be able to use the intel t265 camera, which we currently recommend for 6DOF. This allows northstar developers to reuse the t265 rather than purchase two seperate stereo cameras.&#x20;

### What is the difference between the 25cm, 50cm, and 75cm version of the headset?

These numbers refer to the focal distance that the images appear from the user. The only difference between the two is the location of the screens relative to the combiner. The focal distances can be switched by replacing the display tray. \
\
Typically, we recommend starting with a 25cm build since they are easier to get started with. \
\
&#x20;25cm is sharper for items attached to your hand or right in front of your face.

\- 25cm provides a much better experience when using all the virtual wearable interfaces from Leap Motion

\- 25cm allows for a slightly wider FOV

\- 25cm is also much brighter because of the angle of incidence and collimation layer in the display panel

\- for wandering about and batting stuff around, throwing things, sticking stuff to your wall, or making Characters run around the room 75cm is way more convincing. With 25cm, the Vergence accommodation effect is noticeable, even if you get your IPD just right

\- 75 cm is harder to calibrate than 25cm

In general, 75 cm is better if a lot of things you’re dealing with are further away, while 25 cm is best if you’re prototyping hand interactions. You can still tell how far away things are with either of the focal distances via stereo overall.&#x20;

So to conclude, whichever decision you opt for will work depending on what you plan on doing. However, something to keep in mind is that if you wish to switch between the two display holders, you have to put the screens into the new trays and recalibrate the headset using the stand.

### What are the cables connecting to the display driver board?

There are a total of four cables connecting to the board (see picture below):\
\- two are the two ribbon cables that send data and power to the two displays. The connections are on either side of the board\
&#x20;one for power \
&#x20;one for the mini display port (for transferring data)&#x20;

{% hint style="warning" %}
Note that the board will only work with display port connections that support 4k@60hz, **hdmi -> displayport adapters will not work.** usb-c with displayport functionality or displayport to mini-displayport adapters work fine, as long as they support 4k@60hz.
{% endhint %}

### Troubleshooting Power Requirements

The expected behavior for the board having adequate power and working properly is having yellow led turned on. The LED's location has also been labeled in the image attached above. If there is also a red led lighting up upon plugging it in, this is reflective of insufficient power being fed into the board. If you have a voltmeter handy you might be interested in checking if after plugging the USB into a computer or a wall socket to usb converter that the voltmeter shows 5V of potential. This is the expected output for a USB connection. If the output voltage is correct or if you see no led light lighting whatsoever then there must be an issue with the device driver board. If this is the case reach out in the #noa-labs-display-adapter channel on the discord server for help in debugging.\
\
Secondly, if you do see the board led lighting up but don’t see anything on the screen you can do a couple of things to get things going:

\- plugging the power USB in the same computer where you are plugging your display adapter makes things sync up (as this common source shares the same ground)

\- disconnect everything and connect things in the following order: displays first (make sure you align the pins correctly and don’t force push the cables into the board as that might damage the pins), connect the power cable into your laptop and finally the display adapter. DO NOT under any circumstance unplug the display cables directly from the board while power is being connected. Also, always opt for operating with the cables that connect to the computer

{% hint style="info" %}
**There is a reset button. Using it is actually much safer than plugging cables in and out during testing.**
{% endhint %}
