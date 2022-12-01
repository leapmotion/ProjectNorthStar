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

| Functionality          | Northstar | Hololens 2 | MagicLeap | ARKit   | ARCore   |
| ---------------------- | --------- | ---------- | --------- | ------- | -------- |
| **Device Tracking**    | ✅         | ✅          | ✅         | ✅       | ✅        |
| **Hand Tracking**      | ✅         | ✅          | ✅         | ✅       | ❌        |
| **SteamVR**            | ✅         | ❌          | ❌         | ❌       | ❌        |
| **Plane Tracking**     | ❌         | ✅          | ✅         | ✅       | ✅        |
| **Anchors**            | ✅         | ✅          | ✅         | ✅       | ✅        |
| **Light Estimation**   | ❌         | ❌          | ❌         | ✅       | ✅        |
| **Environment Probes** | ❌         | ❌          |           | ✅       | ✅        |
| **Face tracking**      | ❌         | ❌          | ❌         | ✅       | ✅        |
| **Meshing**            | Zed Mini  | ✅          | ✅         | lidar\* | DepthKit |
| **2D image tracking**  | Aruco     | ✅          | ✅         | ✅       | ✅        |
| **Speech Input**       | ❌         | ✅          | ✅         |         |          |

### Where can I get the lenses and electronic components?

While most of the design is 3d printable, there are components, like the screens, driver board, combiners, and leap motion controller that you will have to order. You can order your parts, upgrade kits, or fully assembled headsets here:&#x20;

{% embed url="https://www.smart-prototyping.com/AR-VR-MR-XR/AR-VR-Kits-Bundles" %}

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
