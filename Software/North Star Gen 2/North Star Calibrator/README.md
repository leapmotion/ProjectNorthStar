# North Star Calibration Routine Gen 2

This is a guide for the new, simplified Calibration routine.  Rather than align the physical components within a North Star to minimize the geometric raytracing error, this calibration instead grafts a camera calibration onto the headset itself.


Before we start, you will need to ensure that you have a:

 - A Project North Star Headset
 - A Project North Star Calibration Stand
 - A Calibration Checkerboard (can be displayed on a monitor)


Note that this new calibration routine does not require an external monitor.

## Calibrating the Stand

 - **captureChessboards.py** 
  - Running this script displays images from your stereo camera.  Show your chessboard to the camera and press **Z** to capture it from various angles.  Get ~30 shots.
  - These images are stored in **./chessboardImages/** as .pngs

 - **viewChessboards.py** 
  - Running this script will simply display the captured images and chessboard patterns on top of each one.  Use this to find misbehaving images where the where the chessboard grid is misaligned between the left and right images.

 - **calibrateChessboards.py** 
  - Running this script will load in your captured chessboard images and calibrate your stereo camera using OpenCV's Fisheye Calibration.


## Calibrating the Headset

 - **captureGraycodes.py**
  - Ensure that your headset is placed on the calibration stand, with the stand's camera looking through it where the users' eyes will be.
  - Additionally ensure that your headset is plugged in and displaying imagery from your desktop.
  - It helps to place a piece of cloth over the rig to shield the cameras + headset from ambient light.
  - Running this script will display a sequence of graycodes on your North Star, capturing them at the same time.
  - The sequence of binary codes will culminate in a 0-1 UV mapping, saved to **"./WidthCalibration.png"** **"./HeightCalibration.png"** in your main folder.

 - **calibrateGraycodes.py**
  - Running this script will fit a 3rd-Degree 2D Polynomial to the left and right "eye"'s X and Y distortions.
  - This polynomial will map from each display's 0-1 UV Coordinates to rectilinear coordinates (where a 3D ray direction is just (x, y, 1.0)).


When you are finished, you may paste the output of the calibrateGraycodes.py into [this diagnostic shadertoy](https://www.shadertoy.com/view/wsscD4) to check for alignment.

Additionally, there should be a `NorthStarCalibration.json` in this directory which you may use in the Unity implementation.
