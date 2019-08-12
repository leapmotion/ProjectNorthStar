---
description: >-
  http://blog.leapmotion.com/project-north-star-mechanical-and-calibration-update-3-1/
---

# Project North Star: Mechanical and Calibration Update 3.1

The future of open source augmented reality just got easier to build. Since our last major release, we’ve streamlined Project North Star even further, including improvements to the calibration system and a simplified optics assembly that 3D prints in half the time. Thanks to feedback from the developer community, we’ve focused on lower part counts, minimizing support material, and reducing the barriers to entry as much as possible. Here’s what’s new with version 3.1.

### Introducing the Calibration Rig

![](http://blog.leapmotion.com/wp-content/uploads/2019/04/calibrationStand.png)As we discussed in our post on the [North Star calibration system](http://blog.leapmotion.com/bending-reality-north-stars-calibration-system/), small variations in the headset’s optical components affect the alignment of the left- and right-eye images. We have to compensate for this in software to produce a convergent image that minimizes eye strain.

Before we designed the calibration stand, each headset would need to have its screen positions and orientations manually compensated for in software. With the North Star calibrator, we’ve automated this step using two visible-light stereo cameras. The optimization algorithm finds the best distortion parameters automatically by comparing images inside the headset with a known reference. This means that auto-calibration can find best possible image quality within a few minutes. Check out our [GitHub project](https://github.com/leapmotion/ProjectNorthStar/tree/master/Software) for instructions on the calibration process.

![](http://blog.leapmotion.com/wp-content/uploads/2019/04/calibration2.gif)![](http://blog.leapmotion.com/wp-content/uploads/2019/04/IMG_8764.jpg)

### Mechanical Updates

Building on feedback from the developer community, we’ve made the assembly easier and faster to put together. Mechanical Update 3.1 introduces a simplified optics assembly, designated \#130-000, that cuts print time in half \(as well as being much sturdier\).

![](http://blog.leapmotion.com/wp-content/uploads/2019/04/Screen-Shot-2019-04-02-at-2.44.55-PM.png)![](http://blog.leapmotion.com/wp-content/uploads/2019/04/IMG_8852.jpg)

The biggest cut in print time comes from the fact that we no longer need support material on the lateral overhangs. In addition, two parts were combined into one. This compounding effect saves an entire workday’s worth of print time!![](http://blog.leapmotion.com/wp-content/uploads/2019/04/IMG_8601.jpg)

Left: 1 part, 95g, 7 hours, no supports. Right: 2 parts, 87g, 15 hour print, supports needed.

The new assembly, \#130-000, is backwards compatible with Release 3. Its components substitute \#110-000 and \#120-000, the optics assembly, and electronics module respectively. Check out the assembly drawings in the [GitHub repo](https://github.com/leapmotion/ProjectNorthStar/tree/master/Mechanical) for the four parts you need!

### Cutout for Power Pins

Last but not least, we’ve made a small cutout for the power pins on the driver board mount. When we received our [NOA Labs driver board](https://www.smart-prototyping.com/Project-North-Star-Display-Driver-Board), we quickly noticed the interference and made the change to all the assemblies.

![](http://blog.leapmotion.com/wp-content/uploads/2019/04/driverboardChange-1.gif)![](http://blog.leapmotion.com/wp-content/uploads/2019/04/powerPinCutouts.jpg)

This change makes it easy if you’re using pins or soldered wires, either on the top or bottom.

Want to stay in the loop on the latest North Star updates? Join the [discussion on Discord](https://discord.gg/NghjdX7)!

