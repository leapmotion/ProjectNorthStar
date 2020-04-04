import numpy as np
import cv2
import time
from tempfile import TemporaryFile
from os import listdir

# The number of images to capture before beginning calibration
numImagesRequired = 10
# The dimension of a single square on the checkerboard in METERS
checkerboardDimension = 0.026 # This equates to 26 millimeter wide squares
# The number of inside corners on the width (long axis) of the checkerboard
checkerboardWidth = 9
# The number of inside corners on the height (short axis) of the checkerboard
checkerboardHeight = 7
# Termination Criteria for the subpixel corner refinement
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

curFrame = 0

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


def loadNextImage():
    global curFrame
    curFrame += 1
    leftFrame  = cv2.imread("./chessboardImages/" + leftFiles[curFrame])
    rightFrame = cv2.imread("./chessboardImages/" + rightFiles[curFrame])
    
    # Detect the Chessboard Corners in the Left Image
    gray = cv2.cvtColor(leftFrame, cv2.COLOR_BGR2GRAY)
    leftDetected, leftCorners = cv2.findChessboardCorners(gray, (checkerboardWidth, checkerboardHeight), None)#, cv2.CALIB_CB_ADAPTIVE_THRESH + cv2.CALIB_CB_NORMALIZE_IMAGE + cv2.CALIB_CB_FAST_CHECK)
    if leftDetected:
        leftCorners = cv2.cornerSubPix(gray, leftCorners, (checkerboardWidth, checkerboardHeight), (-1,-1), criteria)
        #cv2.drawChessboardCorners(leftFrame, (checkerboardWidth, checkerboardHeight), leftCorners, leftDetected)

    # Detect the Chessboard Corners in the Right Image
    gray = cv2.cvtColor(rightFrame, cv2.COLOR_BGR2GRAY)
    rightDetected, rightCorners = cv2.findChessboardCorners(gray, (checkerboardWidth, checkerboardHeight), None)#, cv2.CALIB_CB_ADAPTIVE_THRESH + cv2.CALIB_CB_NORMALIZE_IMAGE + cv2.CALIB_CB_FAST_CHECK)
    if rightDetected:
        rightCorners = cv2.cornerSubPix(gray, rightCorners, (checkerboardWidth, checkerboardHeight), (-1,-1), criteria)
        #cv2.drawChessboardCorners(rightFrame, (checkerboardWidth, checkerboardHeight), rightCorners, leftDetected)

    # Add the detected points to our running arrays when the board is detected in both cameras
    if(leftDetected and rightDetected):
        print(leftCorners.shape)
        print(rightCorners.shape)

        diff = leftCorners - rightCorners
        lengths = np.linalg.norm(diff[:, :, 1], axis=-1)
        sum = np.sum(lengths, axis=0)
        if (sum > 2000.0):
          print("THIS STEREO PAIR IS BROKEN!!! Diff is: "+str(sum))
          rightCorners = np.flipud(rightCorners)
          
        cv2.drawChessboardCorners(leftFrame, (checkerboardWidth, checkerboardHeight), leftCorners, leftDetected)
        cv2.drawChessboardCorners(rightFrame, (checkerboardWidth, checkerboardHeight), rightCorners, leftDetected)

    # Display the resulting frame
    cv2.imshow('Frame L', cv2.resize(leftFrame,  (512,512)))
    cv2.imshow('Frame R', cv2.resize(rightFrame, (512,512)))

loadNextImage()

key = cv2.waitKey(1)
while (not (key & 0xFF == ord('q'))):
    key = cv2.waitKey(1)
    if key & 0xFF == ord('z'):
      loadNextImage()



# When everything is done, release the capture
cv2.destroyAllWindows()

