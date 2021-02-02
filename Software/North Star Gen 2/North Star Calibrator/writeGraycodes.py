import os
import numpy as np
import cv2
import math
import time
import pyrealsense2 as rs2
import intelutils

rigel = False
realsense = True
mock = False

if rigel:
  frameWidth  = 800
  frameHeight = 800
elif realsense:
  frameWidth = 848
  frameHeight = 800
elif mock:
  frameWidth = 512
  frameHeight = 512

northStarSize = (2880, 1600)
if mock:
  northStarSize = (1024, 512)

whiteBrightness = 255

allWhite           = np.ones       ((northStarSize[1], northStarSize[0]), dtype=np.uint8) * whiteBrightness
continuum          = np.arange     (0, 256,         dtype=np.uint8)
continuum          = np.bitwise_xor(continuum, continuum//2) # Binary to Gray
widthContinuum     = np.zeros      (allWhite.shape, dtype=np.uint8)
widthContinuum[:, : int(northStarSize[0] / 2)]   = cv2.resize(continuum[None, :], (int(northStarSize[0] / 2), northStarSize[1]), interpolation=cv2.INTER_NEAREST)
widthContinuum[:,   int(northStarSize[0] / 2) :] = widthContinuum[:, : int(northStarSize[0] / 2)]
heightContinuum    = cv2.resize   (continuum      [:, None      ], northStarSize, interpolation=cv2.INTER_NEAREST)
widthBits          = np.unpackbits(widthContinuum [:,    :, None].astype(np.uint8), axis=-1)
heightBits         = np.unpackbits(heightContinuum[:,    :, None].astype(np.uint8), axis=-1)
widthMeasuredBits  = np.zeros ((frameHeight, frameWidth * 2, 8), dtype=np.uint8)
heightMeasuredBits = np.zeros ((frameHeight, frameWidth * 2, 8), dtype=np.uint8)
lastResult         = np.zeros ((frameHeight, frameWidth * 2))
displayedBuffer    = allWhite - whiteBrightness

cv2.namedWindow      ("Graycode Viewport", 0)#cv2.WINDOW_NORMAL)
cv2.moveWindow       ("Graycode Viewport", 1920, 0)
cv2.setWindowProperty("Graycode Viewport", cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
darkFrameBuffer    = np.zeros((720, 1280), dtype=np.uint8)

#Initialize the Stereo Camera's feed
if rigel:
  cap = cv2.VideoCapture(0)
  cap.set(cv2.CAP_PROP_FRAME_WIDTH , frameWidth)
  cap.set(cv2.CAP_PROP_FRAME_HEIGHT, frameWidth)
  cap.set(cv2.CAP_PROP_CONVERT_RGB , False)
elif realsense:
  print("creating intel thread (at %f)" % time.time())
  cap = intelutils.intelCamThread(frame_callback = lambda frame: None)
  print("starting intel thread (at %f)" % time.time())
  cap.start()
  print("started intel thread (at %f)" % time.time())
elif mock:
  class MockCapture:
    def read(self):
      return True, displayedBuffer
  cap = MockCapture()

if rigel:
  # Turn the Rigel Exposure Up
  os.system(".\LFTool.exe xu set leap 30 " + str(6111) + "L")
# if realsense:

# example

# Initialize 3D Visualizer
frameCount = -3
captureNum = 0
bitField   = 0
stage      = 0
key = cv2.waitKey(1)
while (not (key & 0xFF == ord('q'))):
    key = cv2.waitKey(1)
    print("getting a frame")

    # Capture frame-by-frame
    newFrame, frame = cap.read()
    print("got a frame at %f" % time.time())
    if (newFrame):
        time.sleep(0.1)
        print("got a new frame")
        # Reshape our one-dimensional image into a two-channel side-by-side view of the Rigel's feed
        frame       = np.reshape  (frame, (frameHeight, frameWidth * 2))
        frame_color = cv2.cvtColor(frame , cv2.COLOR_GRAY2BGR)
        bitIndex = int((stage-1)/2)
        if frameCount%6 is 0 and frameCount > 0: #key & 0xFF == ord('z'): #
          if stage is 0:
            # Capture all Black Buffer
            darkFrameBuffer = frame.copy()
            cv2.imwrite(f"./DisplayBuffer/DisplayBuffer_0.png", displayedBuffer)
            # Set Display to White
            displayedBuffer = allWhite
            cv2.imwrite(f"./DisplayBuffer/DisplayBuffer_1.png", displayedBuffer)
          elif stage is 1:
            # Calculate the Monitor Mask and display it
            mask = cv2.threshold(cv2.subtract(frame, darkFrameBuffer), thresh=32, maxval=1, type=cv2.THRESH_BINARY)[1]
            #cv2.imshow("Graycode Display", mask * whiteBrightness)

            # Begin displaying the Width Bits
            displayedBuffer = widthBits [:, :, bitIndex] * whiteBrightness
            cv2.imwrite(f"./DisplayBuffer/DisplayBuffer_{stage+1}.png", displayedBuffer)
          elif stage < 17:
            if stage % 2 is 0:
              darkFrameBuffer = frame.copy()
              displayedBuffer = (1 - widthBits [:, :, bitIndex]) * whiteBrightness
              cv2.imwrite(f"./DisplayBuffer/DisplayBuffer_{stage+1}.png", displayedBuffer)
            else:
              bitmask = cv2.threshold(cv2.subtract(frame, darkFrameBuffer), thresh=1, maxval=1, type=cv2.THRESH_BINARY)[1]
              #cv2.imshow("Graycode Display", bitmask.copy() * mask * whiteBrightness)

              # Add this bitmask to the built up bitmask
              lastResult = bitmask == lastResult # xor with last bitmask - Grey -> binary
              widthMeasuredBits[:, :, bitIndex-1] = lastResult

              displayedBuffer =      widthBits [:, :, bitIndex]  * whiteBrightness
              cv2.imwrite(f"./DisplayBuffer/DisplayBuffer_{stage+1}.png", displayedBuffer)
          elif stage < 33:
            if stage is 17:
              # The Width Bits have finished displaying, we can now pack the graycode bits back into a byte mapping
              calculatedWidthContinuum = np.packbits(widthMeasuredBits, axis=-1)[:, :, 0] * mask
              # This is successful!
              #cv2.imshow("Graycode Width Continuum", cv2.applyColorMap(calculatedWidthContinuum, cv2.COLORMAP_JET))
              cv2.imwrite("./WidthCalibration.png", calculatedWidthContinuum)

              # reset everything - easiest to do here
              lastResult.fill(0)
              darkFrameBuffer.fill(0)
              frame.fill(0)

            if stage % 2 is 0:
              darkFrameBuffer = frame.copy()
              displayedBuffer = (1 - heightBits [:, :, bitIndex-8]) * whiteBrightness
              cv2.imwrite(f"./DisplayBuffer/DisplayBuffer_{stage+1}.png", displayedBuffer)
            else:
              bitmask = cv2.threshold(cv2.subtract(frame, darkFrameBuffer), thresh=1, maxval=1, type=cv2.THRESH_BINARY)[1]
              #cv2.imshow("Graycode Display", bitmask.copy() * mask * whiteBrightness)

              lastResult = bitmask == lastResult # xor with last bitmask - Grey -> binary
              heightMeasuredBits[:, :, bitIndex-9] = lastResult

              displayedBuffer =      heightBits [:, :, bitIndex-8]  * whiteBrightness
              cv2.imwrite(f"./DisplayBuffer/DisplayBuffer_{stage+1}.png", displayedBuffer)

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
        if rigel and (frameCount is 1):
          # Turn the Rigel LED Off
          os.system(".\LFTool.exe xu set leap 27 0")

        frameCount = frameCount + 1

# When everything done, release the capture
cv2.destroyAllWindows()
