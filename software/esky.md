---
description: >-
  Project Esky aims to be the software companion to allow development with the
  Deck X/North Star Display system out of the box! (Utilizing the t265/1 or a
  ZED system for 6DoF tracking)
---

# Esky

This includes a unity package that handles

* Rendering \(with 2D and 3D ****[**Optical Calibration**](../calibration/getting-started/) files\)
* MRTK Integration with the Leap Motion controller \(aligned with the user's view\)
* 6DoF Head Tracking + Re-Localization events \(Save map, load map, add persistence, callbacks, etc.\)
* Spatial mapping \(Via the ZED SDK, with future plans to generate point clouds via the t261/t265 sensor\)

## Relevant Links

### [Project Esky on Github](https://github.com/HyperLethalVector/ProjectEsky-UnityIntegration)

### [Project Esky on Research Gate](https://www.researchgate.net/publication/344337571_Project_Esky_Enabling_High_Fidelity_Augmented_Reality_Content_on_an_Open_Source_Platform)

##  Getting Started with Esky

{% hint style="info" %}
In order to use Esky you'll need Unity, A Northstar headset, and a [2D Calibration](../calibration/2d-calibration.md) or [3D Calibration](../calibration/3d-calibration/) file. 
{% endhint %}

* Copy the contents of your _**NorthStarCalibration.json**_ file into Esky's _**DisplayCalibration.json**_ file Note that the _**DisplayCalibration.json**_ file is located in the root of your unity project folder. Use file explorer in windows to navigate to the _OpticalCalibrations/v2/DisplayCalibration.json_ file 
* Make sure Northstar Display is **not** flipped via windows display manager, additionally it can be helpful to have your Northstar Display setup to the far right of your monitor setup, though if necessary it can be placed anywhere, you'll just need to take note of the position for later.  
* Adjust the _**focusDistance**_ parameter in _**DisplayCalibration.json**_ ****so that it matches your headset's configuration. The number refers to the focal distance trays you have installed.  
* Save the file and then open the _**DisplaySettings.json**_ file, edit the DisplayXLoc and DisplayYLoc to reflect the position of your northstar display relative to your main monitors. For example, if your northstar display is to the right of your main monitor, you'll want to input the monitor's pixel width, for a 4k monitor this would be 3840. 
* Save the DisplaySettings.json file, then open the unity project and click play. You should see a window open on your northstar headset and be able to interact with the MRTK scene provided. 

#### Below is an Example of the _**DisplayCalibration.Json**_ File.

```text
{
    "focusDistance": 0.75,
    "baseline": 0.03200000151991844,
    "left_uv_to_rect_x": [
        -0.7098117476732205,
        1.2394941892905988,
        -0.48417252984476766,
        0.20206420448436016,
        0.03418806429705912,
        0.31907410136307673,
        -0.4605158142547907,
        0.4042603873520616,
        -0.18943883499730696,
        0.48132219553953726,
        -0.30879967889089477,
        -0.13741429189104715,
        0.07547417258600822,
        -0.19519320096721232,
        0.2613765880648765,
        0.06086994286421869
      ],
      "left_uv_to_rect_y": [
        -0.4743021475999604,
        0.5369044594256003,
        -0.8525771689196032,
        0.2627710079276929,
        1.3652046041682682,
        -2.201384532354616,
        3.4438400605675517,
        -1.7580162263564094,
        -0.8231665571360403,
        3.044993768618694,
        -5.194290634899861,
        2.77812666747202,
        0.5755243231226848,
        -1.6224283568386757,
        2.574297012238049,
        -1.3553329073119127
      ],
      "right_uv_to_rect_x": [
        -0.7183962692213749,
        1.6919128983783627,
        -0.8959084856794854,
        0.5118720577498754,
        -0.15182574964167417,
        0.1745194186629323,
        -0.7785485023972123,
        0.23327978620674053,
        0.3893135920142324,
        -1.5912429984917236,
        2.734216996517882,
        -1.1734819917912498,
        -0.2990328028712862,
        1.1872834177169778,
        -1.85802686141548,
        0.8196605383643771
      ],
      "right_uv_to_rect_y": [
        -0.33744873551124743,
        0.1047649025161288,
        0.013610912044989515,
        0.1786419445104183,
        1.192716575734179,
        -0.8489132518356299,
        1.2156256513013421,
        -0.7183342093015473,
        -0.606833960145091,
        1.6425190844725888,
        -3.229817903076172,
        2.1586459013997885,
        0.5037473982889102,
        -1.4667773614252788,
        2.9192654996404515,
        -1.8710955978153174
      ],
    "left_eye_offset": [
        -0.17000000178813935,
        0.15000006556510926
    ],
    "right_eye_offset": [
        -0.18000000715255738,
        0.10000000149011612
    ],
    "localPositionLeapMotion": [
        -0.01632331870496273,
        0.0789,
        0.06120523437857628
    ],
    "localRotationLeapMotion": [
        0.18335089087486268,
        0.0,
        0.0,
        0.9830475449562073
    ]
}
```



