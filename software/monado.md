---
description: Work In Progress documentation on how to setup
---

# Monado

### Installing Third Party Dependencies

#### Intel Realsense

Download librealsense for intel's github repo.

{% embed url="https://github.com/IntelRealSense/librealsense/blob/master/doc/distribution_linux.md" %}

Install using the following commands

```
sh
sudo apt-key adv --keyserver keys.gnupg.net --recv-key F6E65AC044F831AC80A06380C8B3A55A6F3EFCDE || sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-key F6E65AC044F831AC80A06380C8B3A55A6F3EFCDE
sudo add-apt-repository "deb https://librealsense.intel.com/Debian/apt-repo focal main" -u
sudo apt-get install librealsense2-dkms librealsense2-utils librealsense2-dev librealsense2-dbg
```

### Installing Monado Dependencies

```
sudo apt install build-essential cmake libgl1-mesa-dev libvulkan-dev libx11-xcb-dev libxcb-dri2-0-dev libxcb-glx0-dev libxcb-icccm4-dev libxcb-keysyms1-dev libxcb-randr0-dev libxrandr-dev libxxf86vm-dev mesa-common-dev
git clone https://github.com/KhronosGroup/OpenXR-SDK.git
cd OpenXR-SDK
cmake . -G Ninja -DCMAKE_INSTALL_PREFIX=/usr -Bbuild
ninja -C build install
```

```
sh
sudo add-apt-repository ppa:monado-xr/monado
sudo apt-get update
sudo apt install libopenxr-loader1 libopenxr-dev libopenxr-utils build-essential git wget unzip cmake meson ninja-build libeigen3-dev curl patch python3 pkg-config libx11-dev libx11-xcb-dev libxxf86vm-dev libxrandr-dev libxcb-randr0-dev libvulkan-dev glslang-tools 
libglvnd-dev libgl1-mesa-dev ca-certificates libusb-1.0-0-dev libudev-dev libhidapi-dev libwayland-dev libuvc-dev libavcodec-dev libopencv-dev libv4l-dev 
libcjson-dev libsdl2-dev libegl1-mesa-dev 
```

```
sh
git clone https://gitlab.freedesktop.org/monado/monado.git
cd monado
meson build
sudo ninja -C build install -j24
```

### Testing the build

To test that Monado was built properly run the following tests with the example configuration files:&#x20;

```
sh
mkdir -p ~/.config/monado
cp doc/example_configs/config_v0.json.northstar_lonestar ~/.config/monado/config_v0.json
```

```
sh
rs-ar-advanced #test that t26x is working
sudo leapd

NS_CONFIG_PATH=$(pwd)/src/xrt/drivers/north_star/exampleconfigs/v2_deckx_50cm.json monado-service
hello_xr -g Vulkan
```



