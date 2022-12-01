---
description: >-
  This page goes over the general setup you should do after you've finished
  building and calibrating your headset.
---

# Getting Started with Software

### Northstar Development Platforms

There are currently three methods of developing software on Northstar headsets.&#x20;

1. ****[**Esky**](esky.md) **** This is the recommended developer experience. Esky is currently built on Unity and has built in support for the MRTK. It has video passthrough with the t261/t265, temporal reprojection, and supports both the 2D and 3D calibration methods. \

2. &#x20;[**Unity**](software.md) This is the default unity experience, it's barebones and built for the 3D calibration rig. If you're experienced with unity and want to tinker with the original source code for Northstar this is the place for you.\

3. ****[**SteamVR**](steamvr.md) The SteamVR integration allows any SteamVR game to run on a Northstar headset. Hand tracking isn't a replacement for controllers yet so you won't have a fun time in beat saber, but for demos like cat explorer or the infamous cubes demo you'll have full support for hand tracking.\

4. **Prebuilt Examples** There are a handful of prebuilt demos for Northstar including LeapPaint, Galaxies and others. These will be linked on a separate page/database at a future date, for now, [**join the discord**](https://discord.com/invite/NghjdX7) and check the #showcase channel.&#x20;

### Verify your headset is working properly.&#x20;

You'll want to make sure that your headset works properly, plug in the power to the integrator board or driver board first, then the display port connection. On some headsets there can be an issue where plugging the display in first causes the driver board to get enough power through the display port connection to boot, but not enough power to run properly. If you run into this issue unplug your headset and then **plug-in power first, then the display connection.**&#x20;

### **Set Windows Display Settings**

Your Northstar display will show up as a normal monitor and will look "**upside-down**" if viewed through the headset. This is normal, we compensate for this in software written for the headset. Make sure your headset is set up so it's to the **right of your main display(s)**, and that the resolution is 2880x1600. You'll also want to make sure that the scale and layout section is set to 100%. By default, the headset will run at 90hz. You'll also want to ensure that your headset is set to extended mode and not mirrored.&#x20;

![](<../.gitbook/assets/image (8).png>)

### Install Drivers

#### Leap Motion&#x20;

There are a handful of demos that require leap motion's multi-device beta driver, located [**here**](https://github.com/leapmotion/UnityModules/blob/feat-multi-device/Multidevice%20Service/LeapDeveloperKit\_4.0.0%2B52238\_win.zip). If you're having issues with getting your leap working in demos or unity this is probably the reason. Once you have the drivers installed open the leap motion control panel and make sure that the connection is "good", as shown in the figure below.&#x20;

![](<../.gitbook/assets/image (1).png>)

### RealSense SDK

In order to use your t261/t265 sensor you'll need to install the RealSense SDK, located [**here**](https://www.intelrealsense.com/sdk-2/)**.** Once you are finished installing the SDK, open the RealSense viewer application to ensure that your RealSense device is connected. \
\
**If you are using the Deck X headset and your t261 is not showing up you can take the following actions to resolve it:**

1. Push down on both circular buttons on the headset for two seconds to power cycle the integrator board. This should cause the RealSense device to enumerate and cause windows to detect it.&#x20;
2. If step 1 did not work you can try unplugging the headset and plugging it back in, make sure your USB connection is plugged into a USB 3.1 port.&#x20;
3. If both of the above steps did not work you can try resetting the USB hub in device manager. This solution has solved most edge cases we've seen so far.&#x20;

### Northstar Development

There are currently three methods of getting software running on Northstar headsets.&#x20;

1. ****[**Esky**](esky.md) **** This is the recommended developer experience. It has video passthrough with the t261/t265, built-in support for the mixed reality toolkit and support for both 2D and 3D calibration methods.&#x20;
2. &#x20;[**Unity**](software.md) This is the default unity experience, it's barebones and built for the 3D calibration rig. If you're experienced with unity and want to tinker with the original source code for Northstar this is the place for you.
3. ****[**SteamVR**](steamvr.md) The steamvr integration allows any SteamVR game to run on a Northstar headset. Hand tracking isn't a replacement for controllers yet so you won't have a fun time in beat saber, but for demos like cat explorer or the infamous cubes demo you'll have full support for hand tracking a
4. **Prebuilt Examples** There are a handful of prebuilt demos for Northstar including LeapPaint, Galaxies and others. These will be linked on a separate page/database at a future date, for now, [join the discord](https://discord.com/invite/NghjdX7) and check the #showcase channel&#x20;

