import os
import numpy as np
import cv2
import math
import time


frameWidth = 800
northStarSize = (2880, 1600)

allWhite           = np.ones      ((northStarSize[1], northStarSize[0]), dtype=np.uint8) * 100
continuum          = np.arange    (0, 256,         dtype=np.float)
widthContinuum     = np.zeros     (allWhite.shape, dtype=np.float)
widthContinuum[:, : int(northStarSize[0] / 2)]   = cv2.resize(continuum[None, :], (int(northStarSize[0] / 2), northStarSize[1]), interpolation=cv2.INTER_LINEAR_EXACT)
widthContinuum[:,   int(northStarSize[0] / 2) :] = widthContinuum[:, : int(northStarSize[0] / 2)]
heightContinuum    = cv2.resize   (continuum      [:, None      ], northStarSize, interpolation=cv2.INTER_LINEAR_EXACT)
widthBits          = np.unpackbits(widthContinuum [:,    :, None].astype(np.uint8), axis=-1)
heightBits         = np.unpackbits(heightContinuum[:,    :, None].astype(np.uint8), axis=-1)
widthMeasuredBits  = np.zeros ((frameWidth, frameWidth * 2, 8), dtype=np.uint8)
heightMeasuredBits = np.zeros ((frameWidth, frameWidth * 2, 8), dtype=np.uint8)
displayedBuffer    = 100 - allWhite

cv2.namedWindow      ("Graycode Viewport", 0)#cv2.WINDOW_NORMAL)
cv2.setWindowProperty("Graycode Viewport", cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
darkFrameBuffer    = np.zeros((720, 1280), dtype=np.uint8)

#Initialize the Stereo Camera's feed
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH , frameWidth)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, frameWidth)
cap.set(cv2.CAP_PROP_CONVERT_RGB , False)

# Turn the Rigel Exposure Up
os.system(".\LFTool.exe xu set leap 30 " + str(6111) + "L")

# Initialize 3D Visualizer
frameCount = -3
captureNum = 0
bitField   = 0
stage      = 0
key = cv2.waitKey(1)
while (not (key & 0xFF == ord('q'))):
    key = cv2.waitKey(1)
    
    # Capture frame-by-frame
    newFrame, leftRightImage = cap.read()
    if (newFrame):
        # Capture frame-by-frame
        ret, frame = cap.read()

        # Reshape our one-dimensional image into a two-channel side-by-side view of the Rigel's feed
        frame       = np.reshape  (frame, (frameWidth, frameWidth * 2))
        frame_color = cv2.cvtColor(frame , cv2.COLOR_GRAY2BGR)
        bitIndex = int((stage-1)/2)
        if frameCount%6 is 0 and frameCount > 0: #key & 0xFF == ord('z'): #
          if stage is 0:
            # Capture all Black Buffer
            darkFrameBuffer = frame.copy()

            # Set Display to White
            displayedBuffer = allWhite
          elif stage is 1:
            # Calculate the Monitor Mask and display it
            mask = cv2.threshold(cv2.subtract(frame, darkFrameBuffer), thresh=53, maxval=1, type=cv2.THRESH_BINARY)[1]
            #cv2.imshow("Graycode Display", mask * 255)
            
            # Begin displaying the Width Bits
            displayedBuffer = widthBits [:, :, bitIndex] * 255
          elif stage < 17:
            if stage % 2 is 0:
              darkFrameBuffer = frame.copy()
              displayedBuffer = (1 - widthBits [:, :, bitIndex]) * 255
            else:
              bitmask = cv2.threshold(cv2.subtract(frame, darkFrameBuffer), thresh=1, maxval=1, type=cv2.THRESH_BINARY)[1]
              #cv2.imshow("Graycode Display", bitmask.copy() * mask * 255)

              # Add this bitmask to the built up bitmask
              widthMeasuredBits[:, :, bitIndex-1] = bitmask

              displayedBuffer =      widthBits [:, :, bitIndex]  * 255
          elif stage < 33:
            if stage is 17:
              # The Width Bits have finished displaying, we can now pack the graycode bits back into a byte mapping
              calculatedWidthContinuum = np.packbits(widthMeasuredBits, axis=-1)[:, :, 0] * mask
              # This is successful!
              #cv2.imshow("Graycode Width Continuum", cv2.applyColorMap(calculatedWidthContinuum, cv2.COLORMAP_JET))
              cv2.imwrite("./WidthCalibration.png", calculatedWidthContinuum)

            if stage % 2 is 0:
              if stage is not 17:
                darkFrameBuffer = frame.copy()
              displayedBuffer = (1 - heightBits [:, :, bitIndex-8]) * 255
            else:
              bitmask = cv2.threshold(cv2.subtract(frame, darkFrameBuffer), thresh=1, maxval=1, type=cv2.THRESH_BINARY)[1]
              #cv2.imshow("Graycode Display", bitmask.copy() * mask * 255)

              heightMeasuredBits[:, :, bitIndex-9] = bitmask

              displayedBuffer =      heightBits [:, :, bitIndex-8]  * 255
              
          else:
              # The Width Bits have finished displaying, we can now pack the graycode bits back into a byte mapping
              calculatedHeightContinuum = np.packbits(heightMeasuredBits, axis=-1)[:, :, 0] * mask
              # This is successful!
              #cv2.imshow("Graycode Height Continuum", cv2.applyColorMap(calculatedHeightContinuum, cv2.COLORMAP_JET))
              cv2.imwrite("./HeightCalibration.png", calculatedHeightContinuum)
              cv2.destroyAllWindows()
              exit()
              
            # Display the height bits
          stage += 1

        cv2.imshow("Graycode Viewport", displayedBuffer)
        
        # Display the resulting frame
        cv2.imshow('Frame', frame_color)#cv2.resize(frame_color,  (384*2,384)))
        if (frameCount is 1):
          # Turn the Rigel LED Off
          os.system(".\LFTool.exe xu set leap 27 0")

        frameCount = frameCount + 1

# When everything done, release the capture
cv2.destroyAllWindows()