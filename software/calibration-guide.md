---
description: FOLLOW THESE DIRECTIONS TO CALIBRATE YOUR NORTH STAR
---

# Calibration

Take a deep breath and...

## 1. Build the Calibration Rig

* This requires:
  * 3D Printing the mechanical assembly, search for it here: [https://leapmotion.github.io/ProjectNorthStar/](https://leapmotion.github.io/ProjectNorthStar/)
  * Affixing TWO of these Stereo Cameras to it: [https://www.amazon.com/ELP- Industrial- Application- Synchronized- ELP- 960P2CAM- V90- VC/dp/B078TDLHCP/](https://www.amazon.com/ELP-Industrial-Application-Synchronized-ELP-960P2CAM-V90-VC/dp/B078TDLHCP/)
* Acquiring a large monitor to use as the calibration target
* Find the exact model and active area of the screen for this monitor; we'll need it in 3\)

## 2. Calibrate the lenses on your stereo cameras:

* Print out an OpenCV calibration chessboard, and affixing it to a flat backing board.
  * Flatness is absolutely crucial for the calibration.
* Editing the config variables at the top of `dualStereoChessboardCalibration.py` with the correct:
  * Number of interior corners on each axis
  * Dimensions of each square on the checkerboard \(in meters!\)
* Install Python 3 on your machine and run `pip install numpy` and `pip install opencv-python`
  * Run it from the python scripts folder, usually something like `C:\Users\*USERNAME*\AppData\Local\Programs\Python\Python36\Scripts`
* Running `dualStereoChessboardCalibration.py`
  * First, ensure that your upper stereo camera appears on top in the camera visualizer
  * If not, exit the program, and unplug/replug your cameras' USB ports in various orders/ports until they do.
  * Hold your checkerboard in front of your camera array, ensuring to move it around to gain good coverage.
  * Every time the calibrator takes a snapshot, it will print a notice in the terminal.
  * After 30 snapshots in both camera views, it will run the calibration routines and display rectified views.
  * If the calibration went well, you will see your live camera stream rectified such that all straight lines in the real world will appear straight in the camera image, and the images will look straightened and vertically aligned between the screens.
    * If this happened, congratulations!  Exit out of the program and run it one more time.
    * Running it again verifies the calibration can be loaded AND GENERATES THE CALIBRATION JSON
  * If the calibration did not go well \(you see horrible warping and badness\), you can attempt the calibration again by:
    * Deleting the created `dualCameraCalibration.npz` AND `cameraCalibration.json` files from the main folder
    * Trying again: ensuring the checkerboard is flat, the config parameters are correct, and that you have good coverage \(including along depth\)

## 3. Calibrate the Optical Assembly on your North Star

* You should have a good `cameraCalibration.json` file in the main folder \(from the last step\)
* Ensure that your main monitor is 1920x1080 is and the Calibration Monitor appears to the left of the main monitor, and the north star display appears to the right of it.
  * This ensures that the automatic layouting algorithm detects the various monitors appropriately.
* Edit `config.json` to have the active area for your calibration monitor you found earlier.
* Download this version of the Leap Service: [https://github.com/leapmotion/UnityModules/tree/feat-multi-device/Multidevice%20Service](https://github.com/leapmotion/UnityModules/tree/feat-multi-device/Multidevice%20Service)
  * The calibrator was built with this version; it will complain if you don't have it :/
* Now Run the `NorthStarCalibrator.exe`
  * You should see the top camera's images in the top right, and the bottom camera's images on the bottom.
  * If this is not so, please reconnect your cameras until it is \(same process as for the checkerboard script\)
  * You should also see a set of sliders and buttons running along the top.
  * These control the calibration process.
* First, point the bare calibration rig toward the calibration monitor
  * Ensure it is roughly centered on the monitor, so it can see all of the vertical area.
* Then Press "1\) Align Monitor Transform"
  * This will attempt to localize the monitor in space relative to the calibration rig.   This is important for the later steps.
* Next, place the headset onto the calibration rig and press "2\) Create Reflector Mask"
  * This should mask out all of the camera's FoV except the region where the screen and reflectors overlap the calibration monitor.
  * If it does not appear to do this, double check that all of the prior steps have been followed correctly...
* Now, before we press "3\) Toggle Optimization", we'll want to adjust the bottom two sliders until the both represent roughly equal brightnesses.
  * This is important since the optimizer is trying to create a configuration that yields a perfectly gray viewing area.
* Now press "3\) Toggle Optimization" and observe it.
  * It's switching between being valid for the upper and lower camera views, so only one image is going to appear to improve at a time.
  * You should it see it gradually discovering where the aligned camera location is.
  * This is the finickiest step in the process, it's possible that the headset is outside the standard build tolerances.  
    * If you suspect this is the case, increase the simplexSize in the `config.json` to increase the area it will search.
* If it does converge on an aligned image, then congratulations!  Toggle the optimization off again.
* Press button 4\) to hide the pattern, put the headset on, and use the arrow keys to adjust the view dependent/ergonomic distortion and the numpad 2,4,6,8 keys to adjust the rotation of the leap peripheral.
* When satisfied, press 5\) to Save the Calibration
  * This will save your calibration as a "Temp" calibration in the `Calibrations` Folder \(a shortcut is available in the main folder\).
  * You can differentiate between calibrations by the time in which they were created.

