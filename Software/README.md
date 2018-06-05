# Project North Star: Software

This Unity Package contains the Unity Assets, Scenes, and Prewarping systems necessary to build Unity applications with the Project North Star headset.

These assets are dependent on [Release 4.4.0](https://github.com/leapmotion/UnityModules/pull/1010) of the [Leap Motion Unity Modules](https://github.com/leapmotion/UnityModules) (included in the package).

[![North Star Starting Scene](https://i.imgur.com/or9KhyJ.png)](https://github.com/leapmotion/ProjectNorthStar/tree/master/Software)


Getting Started:
  - Make sure your North Star AR Headset is plugged in
  - Create a new Project in Unity 2018.1b13 or above
  - Import "LeapAR.unitypackage"
  - Navigate to `LeapMotion/North Star/Scenes/NorthStar.unity`
  - Click on the `ARCameraRig` game object and look at the `WindowOffsetManager` component
  - Here, you can adjust the X and Y Shift that should be applied to the Unity Game View for it to appear on the North Star's display
  - When you're satisfied with the placement; press "Move Game View To Headset"
  - With the Game View on the Headset, you should be able to preview your experience in play mode!
  - From here you can adjust the headset calibration and placement of the optical elements to refine the predistortion of the headset.
