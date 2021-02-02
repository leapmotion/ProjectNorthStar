import numpy as np
import cv2
import cv2.aruco as aruco
import time
import datetime
from tempfile import TemporaryFile
import json

# CONFIGURATION PARAMETERS

# The number of images to capture before beginning calibration
numImagesRequired = 20
# The dimension of a single square on the checkerboard in METERS
checkerboardDimension = 0.027 # This equates to 27 millimeter wide squares
# The number of inside corners on the width (long axis) of the checkerboard
checkerboardWidth = 9
# The number of inside corners on the height (short axis) of the checkerboard
checkerboardHeight = 6

def writeCalibrationJson(calibrations):
    DeviceCalibrations = {
            "deviceCalibrations":[
                {
                  "source": 0,
                  "date": datetime.datetime.now().strftime("%I:%M%p on %B %d, %Y"),
                  "baseline": float(calibrations[0]["leftToRightTrans"][0])*-1.0,
                  "cameras": [
                    {
                      "distCoeffs":          calibrations[0]["leftDistCoeffs"].flatten().tolist(),
                      "cameraMatrix":        calibrations[0]["leftCameraMatrix"].flatten().tolist(),
                      "rectificationMatrix": calibrations[0]["R1"].flatten().tolist(),
                      "newCameraMatrix":     calibrations[0]["P1"].flatten().tolist()
                    },
                    {
                      "distCoeffs":          calibrations[0]["rightDistCoeffs"].flatten().tolist(),
                      "cameraMatrix":        calibrations[0]["rightCameraMatrix"].flatten().tolist(),
                      "rectificationMatrix": calibrations[0]["R2"].flatten().tolist(),
                      "newCameraMatrix":     calibrations[0]["P2"].flatten().tolist()
                    }
                  ]
                },
                {
                  "source": 1,
                  "date": datetime.datetime.now().strftime("%I:%M%p on %B %d, %Y"),
                  "baseline": float(calibrations[1]["leftToRightTrans"][0])*-1.0,
                  "cameras": [
                    {
                      "distCoeffs":          calibrations[1]["leftDistCoeffs"].flatten().tolist(),
                      "cameraMatrix":        calibrations[1]["leftCameraMatrix"].flatten().tolist(),
                      "rectificationMatrix": calibrations[1]["R1"].flatten().tolist(),
                      "newCameraMatrix":     calibrations[1]["P1"].flatten().tolist()
                    },
                    {
                      "distCoeffs":          calibrations[1]["rightDistCoeffs"].flatten().tolist(),
                      "cameraMatrix":        calibrations[1]["rightCameraMatrix"].flatten().tolist(),
                      "rectificationMatrix": calibrations[1]["R2"].flatten().tolist(),
                      "newCameraMatrix":     calibrations[1]["P2"].flatten().tolist()
                    }
                  ]
                }
            ]
        }
    with open("cameraCalibration.json", "w") as f:
       json.dump(DeviceCalibrations, f, indent=4)

# Initialize some persistent state variables
calibrations = [None, None]
allLeftCorners = [[], []]
allRightCorners = [[], []]
lastTime = [0, 0]

# Check to see if an existing calibration exists
calibrated = False
try:
    loadedCalibration = np.load("dualCameraCalibration.npz")
    calibrated = True
    calibrations = loadedCalibration['calibrations']

except:
    print("No Calibration file found...")

undistortMaps = [None, None]
if(calibrated):
    writeCalibrationJson(calibrations)
    for i in range(2):
        leftUndistortMap = [None, None]
        leftUndistortMap[0], leftUndistortMap[1] = cv2.initUndistortRectifyMap(calibrations[i]['leftCameraMatrix'], calibrations[i]['leftDistCoeffs'], calibrations[i]['R1'], calibrations[i]['P1'], (640,480), cv2.CV_32FC1 )
        rightUndistortMap = [None, None]
        rightUndistortMap[0], rightUndistortMap[1] = cv2.initUndistortRectifyMap(calibrations[i]['rightCameraMatrix'], calibrations[i]['rightDistCoeffs'], calibrations[i]['R2'], calibrations[i]['P2'], (640,480), cv2.CV_32FC1 )

        undistortMaps[i] = (leftUndistortMap, rightUndistortMap)


# Chessboard parameters
# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ...., (checkerboardWidth, checkerboardHeight,0)
objpp = np.zeros((checkerboardHeight*checkerboardWidth,3), np.float32)
objpp[:,:2] = np.mgrid[0:checkerboardWidth,0:checkerboardHeight].T.reshape(-1,2)
objpp = objpp * checkerboardDimension # Set the Object Points to be in real coordinates
objpp = np.asarray([objpp])
objp = np.copy(objpp)
for x in range(numImagesRequired-1):
    objp = np.concatenate((objp, objpp), axis=0)

# Termination Criteria for the subpixel corner refinement
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# Begin webcam capture
cap = [None, None]
for i in range(2):
    cap[i] = cv2.VideoCapture(i)
    cap[i].set(cv2.CAP_PROP_FPS, 30)
    cap[i].set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
    cap[i].set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
    cap[i].set(cv2.CAP_PROP_AUTO_EXPOSURE, 0)
    cap[i].set(cv2.CAP_PROP_EXPOSURE, -7)

webcamImages = [None, None]
while(not (cv2.waitKey(1) & 0xFF == ord('q'))):
    for i in range(2):
        # Capture frame-by-frame
        ret, frame = cap[i].read()
        if(ret and frame is not None):

            leftFrame = frame[:, :int(frame.shape[1]/2)]
            rightFrame = frame[:, int(frame.shape[1]/2):]

            if(calibrated):
                combinedFrames = np.hstack((cv2.remap(leftFrame,  undistortMaps[i][0][0], undistortMaps[i][0][1], cv2.INTER_LINEAR),
                                            cv2.remap(rightFrame, undistortMaps[i][1][0], undistortMaps[i][1][1], cv2.INTER_LINEAR)))

                # Draw Epipolar Lines
                for y in range(int(combinedFrames.shape[0]*0.025)):
                    cv2.line(combinedFrames, (0, y*40), (int(combinedFrames.shape[1]*2), y*40), 255, 1)

            else:
                if(calibrations[i] is None):
                    # Detect the Chessboard Corners in the Left Image
                    gray = cv2.cvtColor(leftFrame, cv2.COLOR_BGR2GRAY)
                    leftDetected, corners = cv2.findChessboardCorners(gray, (checkerboardWidth,checkerboardHeight), None, cv2.CALIB_CB_ADAPTIVE_THRESH + cv2.CALIB_CB_NORMALIZE_IMAGE + cv2.CALIB_CB_FAST_CHECK)
                    if leftDetected:
                        leftCorners = cv2.cornerSubPix(gray, corners,(checkerboardWidth,checkerboardHeight),(-1,-1),criteria)
                        cv2.drawChessboardCorners(leftFrame, (checkerboardWidth,checkerboardHeight), leftCorners, ret)

                    # Detect the Chessboard Corners in the Right Image
                    gray = cv2.cvtColor(rightFrame, cv2.COLOR_BGR2GRAY)
                    rightDetected, corners = cv2.findChessboardCorners(gray, (checkerboardWidth,checkerboardHeight), None, cv2.CALIB_CB_ADAPTIVE_THRESH + cv2.CALIB_CB_NORMALIZE_IMAGE + cv2.CALIB_CB_FAST_CHECK)
                    if rightDetected:
                        rightCorners = cv2.cornerSubPix(gray, corners,(checkerboardWidth,checkerboardHeight),(-1,-1),criteria)
                        cv2.drawChessboardCorners(rightFrame, (checkerboardWidth,checkerboardHeight), rightCorners, ret)

                    # Add the detected points to our running arrays when the board is detected in both cameras
                    if(leftDetected and rightDetected and time.time() - lastTime[i] > 1):
                        allLeftCorners[i].append(leftCorners)
                        allRightCorners[i].append(rightCorners)
                        lastTime[i] = time.time()
                        print("Added Snapshot to array of points")

                    # Once we have all the data we need, begin calibrating!!!
                    if(len(allLeftCorners[i])==numImagesRequired and not calibrated):
                        print("Beginning Left Camera Calibration")
                        leftValid, leftCameraMatrix, leftDistCoeffs, leftRvecs, leftTvecs = cv2.calibrateCamera(objp, allLeftCorners[i], (leftFrame.shape[0], leftFrame.shape[1]), None, None)
                        if(leftValid):
                            print("Left Camera Successfully Calibrated!!")
                            print("Left Camera Matrix:")
                            print(leftCameraMatrix)
                            print("Left Camera Distortion Coefficients:")
                            print(leftDistCoeffs)
                        print("Beginning Right Camera Calibration")
                        rightValid, rightCameraMatrix, rightDistCoeffs, rightRvecs, rightTvecs = cv2.calibrateCamera(objp, allRightCorners[i], (leftFrame.shape[0], leftFrame.shape[1]), None, None)
                        if(rightValid):
                            print("Right Camera Successfully Calibrated!!")
                            print("Right Camera Matrix:")
                            print(rightCameraMatrix)
                            print("Right Camera Distortion Coefficients:")
                            print(rightDistCoeffs)
                        if(leftValid and rightValid):
                            print("WE DID IT, HOORAY!   Now beginning stereo calibration...")
                            valid, leftCameraMatrix, leftDistCoeffs, rightCameraMatrix, rightDistCoeffs, leftToRightRot, leftToRightTrans, essentialMat, fundamentalMat = (
                                cv2.stereoCalibrate(objp, allLeftCorners[i], allRightCorners[i], leftCameraMatrix, leftDistCoeffs, rightCameraMatrix, rightDistCoeffs, (leftFrame.shape[0], leftFrame.shape[1])))

                            if(valid):
                                # Construct the stereo-rectified parameters for display
                                R1, R2, P1, P2, Q, validPixROI1, validPixROI2 = cv2.stereoRectify(leftCameraMatrix,  leftDistCoeffs, 
                                                                                                  rightCameraMatrix, rightDistCoeffs, 
                                                                                                 (leftFrame.shape[0], leftFrame.shape[1]), 
                                                                                                  leftToRightRot, leftToRightTrans)

                                leftUndistortMap = [None, None]
                                leftUndistortMap[0], leftUndistortMap[1] = cv2.initUndistortRectifyMap(leftCameraMatrix, leftDistCoeffs, 
                                                                                                       R1, P1, (leftFrame.shape[1], leftFrame.shape[0]), cv2.CV_32FC1)
                                rightUndistortMap = [None, None]
                                rightUndistortMap[0], rightUndistortMap[1] = cv2.initUndistortRectifyMap(rightCameraMatrix, rightDistCoeffs, 
                                                                                                         R2, P2, (leftFrame.shape[1], leftFrame.shape[0]), cv2.CV_32FC1)
                                undistortMaps[i] = (leftUndistortMap, rightUndistortMap)

                                print("Stereo Calibration Completed!")
                                print("Left to Right Rotation Matrix:")
                                print(leftToRightRot)
                                print("Left to Right Translation:")
                                print(leftToRightTrans)
                                print("Essential Matrix:")
                                print(essentialMat)
                                print("Fundamental Matrix:")
                                print(fundamentalMat)

                                calibrations[i] = {
                                    "leftCameraMatrix":  leftCameraMatrix,
                                    "rightCameraMatrix": rightCameraMatrix,
                                    "leftDistCoeffs":  leftDistCoeffs,
                                    "rightDistCoeffs": rightDistCoeffs,
                                    "leftToRightTrans": leftToRightTrans,
                                    "leftToRightRot": leftToRightRot,
                                    "R1": R1,
                                    "R2": R2,
                                    "P1": P1,
                                    "P2": P2,
                                    }

                                if(calibrations[0] is not None and 
                                   calibrations[1] is not None):
                                    np.savez("dualCameraCalibration.npz", calibrations=calibrations)
                                    calibrated = True
                                    print("Dual Stereo Camera Calibration Completed and Saved!")

                combinedFrames = np.hstack((leftFrame, rightFrame))

            webcamImages[i] = combinedFrames

        # Show Stereo Image
        if(webcamImages[0] is not None and 
           webcamImages[1] is not None):
            cv2.imshow('Combined Frames', np.vstack((webcamImages[0], webcamImages[1])))

# When everything is done, release the capture
cv2.destroyAllWindows()
cap[0].release()
cap[1].release()
