import os
import numpy as np
import cv2
import math
import time

# The number of inside corners on the width (long axis) of the checkerboard
checkerboardWidth = 9
# The number of inside corners on the height (short axis) of the checkerboard
checkerboardHeight = 7
# Checkerboard Dims
checkerboardDims = (checkerboardWidth, checkerboardHeight)
# Termination Criteria for the subpixel corner refinement
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

#Initialize the Stereo Camera's feed
frameWidth = 800
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, frameWidth)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, frameWidth)
cap.set(cv2.CAP_PROP_CONVERT_RGB, False)

# Turn the Rigel Exposure Up
os.system(".\LFTool.exe xu set leap 30 "+str(6111)+"L")

# Initialize 3D Visualizer
frameCount = 0
captureNum = 0
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

        # Detect the Chessboard Corners in the Left Image
        leftDetected, leftCorners = cv2.findChessboardCorners(frame_left, checkerboardDims, None, cv2.CALIB_CB_ADAPTIVE_THRESH + cv2.CALIB_CB_NORMALIZE_IMAGE + cv2.CALIB_CB_FAST_CHECK)
        if leftDetected:
            #leftCorners = cv2.cornerSubPix(frame_left, leftCorners, checkerboardDims, (-1,-1), criteria)
            cv2.drawChessboardCorners(frame_left_color, checkerboardDims, leftCorners, ret)

        # Detect the Chessboard Corners in the Left Image
        rightDetected, rightCorners = cv2.findChessboardCorners(frame_right, checkerboardDims, None, cv2.CALIB_CB_ADAPTIVE_THRESH + cv2.CALIB_CB_NORMALIZE_IMAGE + cv2.CALIB_CB_FAST_CHECK)
        if rightDetected:
            #leftCorners = cv2.cornerSubPix(frame_left, leftCorners, checkerboardDims, (-1,-1), criteria)
            cv2.drawChessboardCorners(frame_right_color, checkerboardDims, rightCorners, ret)

        if key & 0xFF == ord('z') and leftDetected and rightDetected:
            captureNum += 1
            cv2.imwrite("./chessboardImages/leftCapture_" +str(captureNum)+".png", frame_left)
            cv2.imwrite("./chessboardImages/rightCapture_"+str(captureNum)+".png", frame_right)

        # Display the resulting frame
        cv2.imshow('Frame L', cv2.resize(frame_left_color,  (320,320)))
        cv2.imshow('Frame R', cv2.resize(frame_right_color, (320,320)))

        if (frameCount is 10):
          # Turn the Rigel LED Off
          os.system(".\LFTool.exe xu set leap 27 0")
        frameCount = frameCount + 1

# When everything done, release the capture
cv2.destroyAllWindows()