---
description: >-
  Project Esky aims to be the software companion to allow development with the
  Deck X/North Star Display system out of the box! (Utilizing the t265/1 or a
  ZED system for 6DoF tracking)
---

# Esky

Project Esky is a complete unity framework that handles

* Rendering (with 2D and 3D (V1 and V2) **** [**Optical Calibration**](../calibration/getting-started/)**s**)
* MRTK Integration with the Leap Motion controller (aligned with the user's view)
* 6DoF Head Tracking + Re-Localization events (Save map, load map, add persistence, callbacks, etc.)
* Peer to Peer based co-localization and networking at the click of a button (Provided devices are on the same local network)
* Spatial mapping (Via the ZED SDK, with future plans to generate point clouds via the t261/t265 sensor)
* Temporal Reprojection (2D calibration only)

[Project Esky on Github](https://github.com/HyperLethalVector/ProjectEsky-UnityIntegration)\
[Project Esky on Research Gate](https://www.researchgate.net/publication/344337571\_Project\_Esky\_Enabling\_High\_Fidelity\_Augmented\_Reality\_Content\_on\_an\_Open\_Source\_Platform)

## &#x20;Written Guide (Config File Setup)

{% hint style="info" %}
In order to use Esky you'll need Unity, A Northstar headset, and a [2D Calibration](../calibration/2d-calibration.md) or [3D Calibration](../calibration/3d-calibration/) file.&#x20;

Esky runs within the Unreal engine, but the Unity integration is more complete

Esky's Unity Integration uses Unity 2020.3.X, if it complains that you're using a newer version, as long as you're using Unity 2020.3 it should be ok to upgrade. We currently use 2020.3.11f1
{% endhint %}

Note that the _**EskySettings.json**_ file is located in the root of your unity project folder. (Not visible within Unity)

*   For the V2 calibration, Copy the contents of your _**NorthStarCalibration.json**_ file into Esky's _**EskySettings.json**_ file, you will be replacing the following values within the **v2CalibrationValues** section:

    * left\_uv\_to\_rect\_x&#x20;
    * left\_uv\_to\_rect\_y&#x20;
    * right\_uv\_to\_rect\_x&#x20;
    * right\_uv\_to\_rect\_y


*   For the V1 calibration, Copy the contents of your NorthStarCalibration.json file into Esky's&#x20;

    _**EskySettings.json**_ file, you will be replacing all of the values within the **v1CalibrationValues** section\

* Make sure Northstar Display is **not** flipped via windows display manager, additionally it can be helpful to have your Northstar Display setup to the far right of your monitor setup, though if necessary it can be placed anywhere, you'll just need to take note of the position for later. \

*   Then, within the EskySettings.Json file, edit the displayWindowSettings so that the DisplayXLoc and DisplayYLoc  reflect the position of your northstar display relative to your main monitors.&#x20;

    * In the below example, Monitor #2 is my NorthStar, and Monitor #3 is marked as my 'Main Display'. Since my 'Main Display' is 1920x1080 pixels in size, my DisplayXLoc and DisplayYLoc values will be 0, and 1080, respectively

    ![](<../.gitbook/assets/image (12).png>)

    \

* Save the EskySettings.json file. You're now free to proceed to open the Unity Project to complete the calibration

## Hand Alignment and MRTK Setup

Before you can begin doing cool northstar stuff, you'll need to align your hands so that the virtual image matches the real world, and configure the MRTK to your specific North Star Setup.

**For V1:**

*   Observe the following section of your V1 (3D) calibration json:\


    ```
    "leapTracker": {
        "name": "Leap Odometry Origin",
        "serial": "",
        "localPose": {
          "position": {
            "x": 0.0,
            "y": 0.039777886122465137,
            "z": 0.0804821103811264
          },
          "rotation": {
            "x": 0.30401188135147097,
            "y": -0.016626091673970224,
            "z": 0.00530654564499855,
            "w": 0.9525083899497986
          }
        }
      }
    ```
* You will need to copy the position, and rotation values into the following area within the EskySettings.json file. the position maps to TranslationEyeToLeapMotion, and the rotation maps to RotationEyeToLeapMotion.
  * ```
    "myOffsets": {
            "TranslationEyeToLeapMotion": {
                "x": -0.002040371298789978,
                "y": 0.07525337487459183,
                "z": 0.09259902685880661
            },
            "RotationEyeToLeapMotion": {
                "x": 0.04397745057940483,
                "y": 0.02938256785273552,
                "z": -0.08629350364208222,
                "w": 0.9948649406433106
            },
    }
    ```
* Save when complete, you can see an example of a completed EskySettings.json file in the Extras section

**For V2:**

* Open the Unity project, then navigate to [Assets](https://github.com/HyperLethalVector/ProjectEsky-UnityIntegration/tree/master/Assets)/[Scenes](https://github.com/HyperLethalVector/ProjectEsky-UnityIntegration/tree/master/Assets/Scenes)/[Calibration](https://github.com/HyperLethalVector/ProjectEsky-UnityIntegration/tree/master/Assets/Scenes/Calibration)/**LMHandCalibration.unity**
* Hit play, then, click on the unity Game window (so that unity can receive the keyboard input)
* Alignment is pretty straightforward, following the instructions displayed in your headset:
  * 1\) Hold your right hand up, so that the virtual hand appears in the center of the screen
  * 2\) Hold the space bar
  * 3\) While holding space, translate your right hand so that it aligns with the frozen virtual hand, \
    **NOTE: Try not to move your fingers as you do this**
  * 4\) Once aligned, release the space bar
  * 5\) Repeat steps 1-4 3 or 4 times, the alignment system will let you know when it has collected enough samples.&#x20;
  * If you are happy with the alignment, hit 's' to save, if not, stop unity (hit the play button again) then hit play to start the process again, repeating steps 1-5
  * Further adjustment can be done with the arrow keys

## Setting up the MRTK Example

Now we get to the fun stuff! As of RC1, the MRTK handles all of the configuration for Project Esky!\
Open Project Esky in Unity, then navigate to [Assets](https://github.com/HyperLethalVector/ProjectEsky-UnityIntegration/tree/master/Assets)/[Scenes](https://github.com/HyperLethalVector/ProjectEsky-UnityIntegration/tree/master/Assets/Scenes)/[Examples](https://github.com/HyperLethalVector/ProjectEsky-UnityIntegration/tree/master/Assets/Scenes/Examples)/**HandInteractionExamples.unity**

If you look at the Scene Heirarchy, you will notice the MixedRealityToolkit gameobject.&#x20;



![](<../.gitbook/assets/image (10).png>)

The inspector will show the Mixed Reality Toolkit configuration. Click Input -> Input Data Providers\


![](<../.gitbook/assets/image (3).png>)

You will see the following window for configuring all of esky's settings.\


![](<../.gitbook/assets/image (24).png>)

The settings are explained as follows:

**Rig To Use:** This controls which optical setup is used for your northstar (V1, V2), Project Ariel, or a custom rig can be selected.\
\
**Filter System to Use:** We work hard to develop Project Esky, and while we have a newer (and in our opinion better designed) pose filtering system, you can change this value to revert back to the old filtering pipeline here\
\
**Reprojection Settings (V2/Ariel Only):** This enables/disables the late-stage temporal reprojection built into Project Esky's native rendering pipeline.

**Native Shader To Use (V2/Ariel Only, Currently not implemented):** This changes the undistortion method used by the native rendering pipeline, we recommend not editing this.

**Target Frame Rate (V2/Ariel Only):** This changes the target frame rate for Unity. You can select 120, 90, 60, and 30 frames per second!\
**NOTE: This frame rate is independent of the native rendering pipeline that handles composition, which always runs at 120 frames per second!**

**Leap Controller Orientation:** This changes the way the MRTK handles the leapmotion controller, we recommend leaving this as 'Esky'

**Enter/Exit pinch distance:** This changes the distance between the index and thumb before the MRTK considers a pinch start/finish action (in meters).

**Save after stopping editor:** The default behaviour of Project Esky is to dump your current MRTK settings back into the EskySettings.json file, which is also copied to any build directory when you built your unity project. You can disable this behaviour but we don't recommend it!

**Use Tracker Offsets (Not Implemented):** This places the virtual camera relative to the 6DoF tracker, good for external between-sensor calibrations.

**Uses Camera Preview:** This changes whether you intend to use image passthrough preview with the t261, keep in mind you must have **USB 3.0 connected to your NorthStar in order to use it!**

**Uses External RGB Camera:** For those with an RGB sensor, you can enable this to use the RGB image passthrough, unless you know what you're doing **Keep this disabled!!**

## All Done?

Then simply hit 'play' and you're good to go :D

## Extras

#### Below is an Example of my complete _**EskySettings.Json**_ File, with the updated V1 and V2 calibration for a 25cm focal length optical setup.

**Note: Some of the values you see are either controlled by the MRTK in editor, or not yet in use.**

```
{
    "rigToUse": 0,
    "targetFrameRate": 120,
    "reprojectionSettings": 0,
    "nativeShaderToUse": 0,
    "myOffsets": {
        "TranslationEyeToLeapMotion": {
            "x": -0.002040371298789978,
            "y": 0.07525337487459183,
            "z": 0.09259902685880661
        },
        "RotationEyeToLeapMotion": {
            "x": 0.04397745057940483,
            "y": 0.02938256785273552,
            "z": -0.08629350364208222,
            "w": 0.9948649406433106
        },
        "TranslationFromTracker": {
            "x": 0.0,
            "y": 0.0,
            "z": 0.0
        },
        "RotationFromTracker": {
            "x": 0.0,
            "y": 0.0,
            "z": 0.0,
            "w": 1.0
        },
        "RGBSensorTranslationFromTracker": {
            "x": 0.0,
            "y": 0.0,
            "z": 0.0
        },
        "RGBSensorRotationFromTracker": {
            "x": 0.0,
            "y": 0.0,
            "z": 0.0,
            "w": 1.0
        }
    },
    "v2CalibrationValues": {
        "left_uv_to_rect_x": [
            -0.6252081394195557,
            0.3324834108352661,
            1.299890398979187,
            -0.8253355622291565,
            -1.0325636863708497,
            7.521162033081055,
            -13.799334526062012,
            7.8558759689331059,
            2.1261489391326906,
            -14.113080978393555,
            26.34162139892578,
            -14.930045127868653,
            -1.3233585357666016,
            8.506293296813965,
            -15.606451034545899,
            8.893977165222168
        ],
        "left_uv_to_rect_y": [
            -0.37523153424263,
            -0.34262487292289736,
            0.7635243535041809,
            -0.6682243347167969,
            0.38151007890701296,
            4.049591541290283,
            -7.806105136871338,
            4.473989486694336,
            1.0507723093032838,
            -8.660529136657715,
            15.536046981811524,
            -8.565287590026856,
            -0.492713987827301,
            4.902211666107178,
            -8.764571189880371,
            4.757476329803467
        ],
        "right_uv_to_rect_x": [
            -0.7759054899215698,
            1.748594880104065,
            -0.8957473039627075,
            0.5134794116020203,
            -0.06296022981405258,
            0.5289550423622131,
            -1.9657267332077027,
            0.9775575399398804,
            0.34445589780807497,
            -2.996290445327759,
            6.617568016052246,
            -3.72568416595459,
            -0.33747774362564089,
            2.3275089263916017,
            -4.799829483032227,
            2.7676336765289308
        ],
        "right_uv_to_rect_y": [
            -0.40155869722366335,
            0.0788295716047287,
            0.18848803639411927,
            0.03440302610397339,
            1.162545919418335,
            -0.0871603861451149,
            -0.47248953580856326,
            0.2951107323169708,
            -0.28124600648880007,
            -0.8912697434425354,
            1.6107137203216553,
            -0.6361001133918762,
            0.19606836140155793,
            0.5201933979988098,
            -0.8043093681335449,
            0.28155842423439028
        ],
        "left_eye_offset": [
            0.0,
            0.0
        ],
        "right_eye_offset": [
            0.0,
            0.0
        ]
    },
    "v1CalibrationValues": {
        "leftEye": {
            "ellipseMinorAxis": 0.24494899809360505,
            "ellipseMajorAxis": 0.3099985122680664,
            "screenForward": {
                "x": 0.5140379071235657,
                "y": -0.2891080379486084,
                "z": 0.8075775504112244
            },
            "screenPosition": {
                "x": -0.07188151031732559,
                "y": -0.008429261855781079,
                "z": 0.026248455047607423
            },
            "eyePosition": {
                "x": -0.029943088069558145,
                "y": -0.003366167191416025,
                "z": -0.00427968380972743
            },
            "eyeRotation": {
                "x": 7.395533370627759e-32,
                "y": 1.7763568394002506e-15,
                "z": 1.1241009818395605e-14,
                "w": 1.0
            },
            "cameraProjection": {
                "x": -0.699999988079071,
                "y": 0.699999988079071,
                "z": 0.30000001192092898,
                "w": -1.399999976158142
            },
            "sphereToWorldSpace": {
                "e00": 0.05101080238819122,
                "e01": 0.06071346998214722,
                "e02": 0.29330453276634219,
                "e03": -0.11233785003423691,
                "e10": 0.0,
                "e11": 0.23695310950279237,
                "e12": -0.07855913043022156,
                "e13": 0.026850363239645959,
                "e20": -0.23957861959934236,
                "e21": 0.012927045114338398,
                "e22": 0.062450066208839419,
                "e23": -0.046845193952322009,
                "e30": 0.0,
                "e31": 0.0,
                "e32": 0.0,
                "e33": 1.0
            },
            "worldToScreenSpace": {
                "e00": 1.8454549312591553,
                "e01": 14.493172645568848,
                "e02": 4.013805866241455,
                "e03": 0.14946463704109193,
                "e10": -14.294207572937012,
                "e11": -0.6365562677383423,
                "e12": 8.87063980102539,
                "e13": -1.265695571899414,
                "e20": 0.5140379071235657,
                "e21": -0.2891080975532532,
                "e22": 0.8075775504112244,
                "e23": 0.013315193355083466,
                "e30": 0.0,
                "e31": 0.0,
                "e32": 0.0,
                "e33": 1.0
            }
        },
        "rightEye": {
            "ellipseMinorAxis": 0.24494899809360505,
            "ellipseMajorAxis": 0.3099985122680664,
            "screenForward": {
                "x": -0.42177313566207888,
                "y": -0.18166834115982057,
                "z": 0.8883152604103088
            },
            "screenPosition": {
                "x": 0.07054391503334046,
                "y": -0.009491059929132462,
                "z": 0.028982438147068025
            },
            "eyePosition": {
                "x": 0.029943088069558145,
                "y": -0.003366167191416025,
                "z": -0.00427968380972743
            },
            "eyeRotation": {
                "x": 7.395533370627759e-32,
                "y": 1.7763568394002506e-15,
                "z": 1.1241009818395605e-14,
                "w": 1.0
            },
            "cameraProjection": {
                "x": -0.699999988079071,
                "y": 0.699999988079071,
                "z": 0.30000001192092898,
                "w": -1.399999976158142
            },
            "sphereToWorldSpace": {
                "e00": 0.05101081356406212,
                "e01": -0.060713451355695727,
                "e02": -0.29330453276634219,
                "e03": 0.11538401991128922,
                "e10": 0.0,
                "e11": 0.23695312440395356,
                "e12": -0.07855910062789917,
                "e13": 0.025588704273104669,
                "e20": 0.23957861959934236,
                "e21": 0.0129270413890481,
                "e22": 0.062450066208839419,
                "e23": -0.04654233157634735,
                "e30": 0.0,
                "e31": 0.0,
                "e32": 0.0,
                "e33": 1.0
            },
            "worldToScreenSpace": {
                "e00": -1.1463184356689454,
                "e01": 14.899345397949219,
                "e02": 2.502774715423584,
                "e03": 0.1497398465871811,
                "e10": -15.211103439331055,
                "e11": 0.041456401348114017,
                "e12": -7.213770389556885,
                "e13": 1.2825169563293458,
                "e20": -0.42177313566207888,
                "e21": -0.18166831135749818,
                "e22": 0.8883152604103088,
                "e23": 0.002283765934407711,
                "e30": 0.0,
                "e31": 0.0,
                "e32": 0.0,
                "e33": 1.0
            }
        }
    },
    "displayWindowSettings": {
        "DisplayXLoc": 0,
        "DisplayYLoc": 1080,
        "DisplayWidth": 2880,
        "DisplayHeight": 1600,
        "EyeTextureWidth": 1600,
        "EyeTextureHeight": 1440
    },
    "sensorModuleCalibrations": {
        "camID": 0,
        "fx": 0.0,
        "fy": 0.0,
        "cx": 0.0,
        "cy": 0.0,
        "d1": 0.0,
        "d2": 0.0,
        "d3": 0.0,
        "d4": 0.0,
        "SensorWidth": 0,
        "SensorHeight": 0,
        "SensorChannels": 0,
        "SensorFoV": 0.0
    },
    "usesExternalRGBCamera": false,
    "UsesCameraPreview": true,
    "UseTrackerOffsets": true
}
```

