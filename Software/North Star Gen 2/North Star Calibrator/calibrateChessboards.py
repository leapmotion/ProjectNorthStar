import numpy as np
import cv2
import time
from tempfile import TemporaryFile
from os import listdir

# CONFIGURATION PARAMETERS

# The number of images to capture before beginning calibration
numImagesRequired = 10
# The dimension of a single square on the checkerboard in METERS
checkerboardDimension = 0.0266 # This equates to 26 millimeter wide squares
# The number of inside corners on the width (long axis) of the checkerboard
checkerboardWidth = 9
# The number of inside corners on the height (short axis) of the checkerboard
checkerboardHeight = 7

# Chessboard parameters
# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ...., (checkerboardWidth, checkerboardHeight,0)
objpp = np.zeros((checkerboardHeight*checkerboardWidth,3), np.float32)
objpp[:,:2] = np.mgrid[0:checkerboardWidth,0:checkerboardHeight].T.reshape(-1,2)
objpp = objpp * checkerboardDimension # Set the Object Points to be in real coordinates
objpp = np.asarray([objpp])
objp = np.copy(objpp)

# Termination Criteria for the subpixel corner refinement
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# Initialize some persistent state variables
allLeftCorners = []
allRightCorners = []
lastTime = 0
calibrated = False

# Get Images in the directory
imageFiles = listdir("./chessboardImages/")
leftFiles =[]
rightFiles = []
for file in imageFiles:
  if "left" in file:
    leftFiles.append(file)
  if "right" in file:
    rightFiles.append(file)
leftFiles  = sorted(leftFiles)
rightFiles = sorted(rightFiles)

for x in range(len(leftFiles)-1):
    objp = np.concatenate((objp, objpp), axis=0)
objp = objp[:, :, None, :]
print(objp.shape)


for leftFile, rightFile in zip(leftFiles, rightFiles):
        leftFrame  = cv2.imread("./chessboardImages/" + leftFile)
        rightFrame = cv2.imread("./chessboardImages/" + rightFile)

        # Detect the Chessboard Corners in the Left Image
        gray = cv2.cvtColor(leftFrame, cv2.COLOR_BGR2GRAY)
        leftDetected, leftCorners = cv2.findChessboardCorners(gray, (checkerboardWidth, checkerboardHeight), None)#, cv2.CALIB_CB_ADAPTIVE_THRESH + cv2.CALIB_CB_NORMALIZE_IMAGE + cv2.CALIB_CB_FAST_CHECK)
        if leftDetected:
            leftCorners = cv2.cornerSubPix(gray, leftCorners, (checkerboardWidth, checkerboardHeight), (-1,-1), criteria)
            #cv2.drawChessboardCorners(leftFrame, (checkerboardWidth, checkerboardHeight), leftCorners, leftDetected)

        # Detect the Chessboard Corners in the Right Image
        gray = cv2.cvtColor(rightFrame, cv2.COLOR_BGR2GRAY)
        rightDetected, rightCorners = cv2.findChessboardCorners(gray, (checkerboardWidth, checkerboardHeight), None) #, cv2.CALIB_CB_ADAPTIVE_THRESH + cv2.CALIB_CB_NORMALIZE_IMAGE + cv2.CALIB_CB_FAST_CHECK
        if rightDetected:
            rightCorners = cv2.cornerSubPix(gray, rightCorners, (checkerboardWidth, checkerboardHeight), (-1,-1), criteria)
            #cv2.drawChessboardCorners(rightFrame, (checkerboardWidth, checkerboardHeight), rightCorners, rightDetected)

        # Add the detected points to our running arrays when the board is detected in both cameras
        if (leftDetected and rightDetected):
            # Check for the flipped checkerboard!
            diff = np.abs(leftCorners - rightCorners)[:, :, 1]
            #lengths = np.linalg.norm(diff[:, :, 1], axis=-1)
            sum = np.sum(diff, axis=0)
            if (sum > 2000.0):
                print("THIS STEREO PAIR IS BROKEN!!! Diff is: "+str(sum))
                rightCorners = np.flipud(rightCorners)

            allLeftCorners.append(leftCorners)
            allRightCorners.append(rightCorners)
            lastTime = time.time()
            print("Added Snapshot from image "+leftFile+" to array of points")

        # Once we have all the data we need, begin calibrating!!!
        if (len(allLeftCorners) == len(leftFiles) and not calibrated):
            print("Beginning Left Camera Calibration")
            leftValid,  leftCameraMatrix,  leftDistCoeffs,  leftRvecs,  leftTvecs  = cv2.fisheye.calibrate(objp, allLeftCorners,  (leftFrame.shape[0],  leftFrame.shape[1]),  None, None)
            print("Beginning Right Camera Calibration")
            rightValid, rightCameraMatrix, rightDistCoeffs, rightRvecs, rightTvecs = cv2.fisheye.calibrate(objp, allRightCorners, (rightFrame.shape[0], rightFrame.shape[1]), None, None)
            if(leftValid):
                print("Left Camera Successfully Calibrated!!")
                print("Left Camera Matrix:")
                print(leftCameraMatrix)
                print("Left Camera Distortion Coefficients:")
                print(leftDistCoeffs)
            if(rightValid):
                print("Right Camera Successfully Calibrated!!")
                print("Right Camera Matrix:")
                print(rightCameraMatrix)
                print("Right Camera Distortion Coefficients:")
                print(rightDistCoeffs)
            if(leftValid and rightValid):
                print("WE DID IT, HOORAY!   Now beginning stereo calibration...")
                valid, leftCameraMatrix, leftDistCoeffs, rightCameraMatrix, rightDistCoeffs, leftToRightRot, leftToRightTrans= (
                    cv2.fisheye.stereoCalibrate(objp, allLeftCorners, allRightCorners, leftCameraMatrix, leftDistCoeffs, rightCameraMatrix, rightDistCoeffs, (leftFrame.shape[0], leftFrame.shape[1]), None, None))

                if(valid):
                    # Construct the stereo-rectified parameters for display
                    R1, R2, P1, P2, Q = cv2.fisheye.stereoRectify(leftCameraMatrix,   leftDistCoeffs, 
                                                                   rightCameraMatrix,  rightDistCoeffs, 
                                                                  (leftFrame.shape[0], leftFrame.shape[1]),
                                                                   leftToRightRot,     leftToRightTrans,
                                                                   0,                 (leftFrame.shape[1], leftFrame.shape[0]))
                    #R1 = np.eye(3)
                    #R2 = np.eye(3)
                    #P1 = leftCameraMatrix
                    #P2 = rightCameraMatrix

                    leftUndistortMap  = [None, None]
                    leftUndistortMap[0], leftUndistortMap[1]   = cv2.fisheye.initUndistortRectifyMap(leftCameraMatrix, leftDistCoeffs, 
                                                                                                     R1, P1, (leftFrame.shape[1], leftFrame.shape[0]), cv2.CV_32FC1)
                    rightUndistortMap = [None, None]
                    rightUndistortMap[0], rightUndistortMap[1] = cv2.fisheye.initUndistortRectifyMap(rightCameraMatrix, rightDistCoeffs, 
                                                                                                     R2, P2, (leftFrame.shape[1], leftFrame.shape[0]), cv2.CV_32FC1)
        
                    print("Stereo Calibration Completed!")
                    print("Left to Right Rotation Matrix:")
                    print(leftToRightRot)
                    print("Left to Right Translation:")
                    print(leftToRightTrans)
                    np.savez("cameraCalibration.npz", leftCameraMatrix  = leftCameraMatrix,  leftDistCoeffs   = leftDistCoeffs, 
                                                      rightCameraMatrix = rightCameraMatrix, rightDistCoeffs  = rightDistCoeffs, 
                                                      R1 = R1, R2 = R2, baseline = np.linalg.norm(leftToRightTrans))
                    calibrated = True

# Now that we're calibrated, let's look at what those calibrated images look like...

#Initialize the Stereo Camera's feed
frameWidth = 800
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, frameWidth)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, frameWidth)
cap.set(cv2.CAP_PROP_CONVERT_RGB, False)

key = cv2.waitKey(1)
while (not (key & 0xFF == ord('q'))):
    key = cv2.waitKey(1)
    # Capture frame-by-frame
    newFrame, leftRightImage = cap.read()
    if (newFrame):
      
        # Capture frame-by-frame
        ret, frame = cap.read()

        # Reshape our one-dimensional image into a two-channel side-by-side view of the Rigel's feed
        frame             = np.reshape(frame, (frameWidth, frameWidth * 2))
        frame_left        = frame[:, :frameWidth]
        frame_right       = frame[:, frameWidth:]
        frame_left_color  = cv2.cvtColor(frame_left , cv2.COLOR_GRAY2BGR)
        frame_right_color = cv2.cvtColor(frame_right, cv2.COLOR_GRAY2BGR)

        # Display the Marker Tracking Overlay
        stereoImages = []
        if(calibrated):
            stereoImages.append(cv2.remap(frame_left_color,  leftUndistortMap[0],  leftUndistortMap[1],  cv2.INTER_LINEAR))
            stereoImages.append(cv2.remap(frame_right_color, rightUndistortMap[0], rightUndistortMap[1], cv2.INTER_LINEAR))
        else:
            stereoImages.append(frame_left_color)
            stereoImages.append(frame_right_color)
        combinedFrames = np.hstack((stereoImages[0], stereoImages[1]))

        # Draw Epipolar Lines
        for y in range(int(combinedFrames.shape[0]*0.025)):
            cv2.line(combinedFrames, (0, y*40), (int(combinedFrames.shape[1]*2), y*40), 255, 1)

        # Show Stereo Image
        cv2.imshow('Combined Frame', combinedFrames)

# When everything is done, release the capture
cv2.destroyAllWindows()
cap.release()
