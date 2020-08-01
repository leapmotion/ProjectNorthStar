# FAQ

### What is Project North Star?

Project North Star is an open source augmented reality headset meant as a development platform for interaction prototyping it was initially released by leap motion in 2018 and continues to recieve updates. 

### How can I build it?

Good Question! There are multiple variations of the reference design from leap motion. The reference design can be found here: 

{% embed url="https://github.com/leapmotion/ProjectNorthStar" caption="Link to the main GitHub repo" %}

### Where can I get the lenses and electronic components?

While most of the design is 3d printable, there are components, like the screens, driver board, combiners, and leap motion controller that you will have to order. You can order your parts, or fully assembled headsets here: 

{% embed url="https://www.smart-prototyping.com/AR-VR-MR-XR/Project-North-Star-Lens" %}

### What 3d printer should I get?

Some of the parts for project northstar are a bit large, the ender series by creality seems to be a fan favorite among the discord if you're just getting started with 3d printing. If you want something that has a larger print area, check out the creality pro. If you want other recommendations, feel free to ask on the discord.

{% embed url="https://www.creality3d.shop/collections/3d-printer/products/creality-ender-3-3d-printer-economic-ender-diy-kits-with-resume-printing-function-v-slot-prusa-i3-220x220x250mm" caption="Ender3d" %}

{% embed url="https://www.creality3d.shop/collections/3d-printer/products/creality-cr-10s-diy-desktop-3d-printer-300x300x400mm" caption="Creality Pro" %}

### How can I track my position?

The Intel RealSense T265 is the most commonly used device currently. It supports 6dof \(degrees of freedom\) but does not support world meshing. 

~~The occipital structure core is great since it's cross platform and non GPU dependent and has more features, but it's more expensive than the Realsense. \(Note that if you order this you need the black and white camera version and NOT the color version\). There are members of occipital here in the discord to answer more questions, check out the \#occipital-structure-core channel.~~ **Occipital has discontinued support of the perception engine and is no longer recommended.** 

If you have a windows PC with a 1070 or above you can use the Zed Mini, but it only works with Nvidia CUDA which limits its use.

If you have a Vive already, you can use a vive tracker for 6dof tracking, however the vive tracker requires external "lighthouse" base stations in order to function, making it more difficult to transport the headset or use it in different environments without extra setup.

