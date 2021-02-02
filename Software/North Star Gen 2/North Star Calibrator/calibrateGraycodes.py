import os
import numpy as np
import cv2
import math
import time
import traceback
import sys
import json
np.set_printoptions(suppress = True, precision=8)

rigel = False
realsense = True

# Check to see if an existing calibration exists
calibrated = False
try:
    calibrations = np.load(f".\cameraCalibration{'_rs' if realsense else ''}.npz", )
    calibrated = True
except:
    traceback.print_exc(file=sys.stdout)
    print("Calibration file failed to load!")
    exit()

# Load in the Per-Pixel Homographies from the last step
widthData  = cv2.cvtColor(cv2.imread("./WidthCalibration.png"),  cv2.COLOR_BGR2GRAY)
heightData = cv2.cvtColor(cv2.imread("./HeightCalibration.png"), cv2.COLOR_BGR2GRAY)
rawData    = np.zeros((widthData.shape[0], widthData.shape[1], 3), dtype=np.uint8)
rawData[..., 2] = widthData; rawData[..., 1] = heightData
leftData   = rawData [:, : int(rawData.shape[1] / 2)  ]
rightData  = rawData [:,   int(rawData.shape[1] / 2) :]

outputCalibration = { "baseline": 64.0,
'left_uv_to_rect_x' : None,  'left_uv_to_rect_y': None,
'right_uv_to_rect_x': None, 'right_uv_to_rect_x': None }

def polyfit2d(X, Y, Z, deg):
  """Fit a 3D polynomial of degree deg"""
  vander = np.polynomial.polynomial.polyvander2d(X, Y, deg)
  vander = vander.reshape((-1, vander.shape[-1]))
  c      = np.linalg.lstsq(vander, Z.reshape((vander.shape[0],)), rcond=-1)[0]
  return   c.reshape(deg + 1)

def polyval2d(X, Y, C):
  """Evaluate a 2D Polynomial with coefficients C"""
  output = np.zeros_like(X)
  for i in range(C.shape[0]):
    for j in range(C.shape[1]):
      output += C[i, j] * (X ** i) * (Y ** j)
  return output

def polyval2dExpanded(X, Y, C):
  """Evaluate a 2D Polynomial with coefficients C, but more verbose"""
  Cf = C.flatten()
  X2 = X * X; X3 = X2 * X
  Y2 = Y * Y; Y3 = Y2 * Y
  return (((Cf[ 0]     ) + (Cf[ 1]      * Y) + (Cf[ 2]      * Y2) + (Cf[ 3]      * Y3)) +
          ((Cf[ 4] * X ) + (Cf[ 5] * X  * Y) + (Cf[ 6] * X  * Y2) + (Cf[ 7] * X  * Y3)) +
          ((Cf[ 8] * X2) + (Cf[ 9] * X2 * Y) + (Cf[10] * X2 * Y2) + (Cf[11] * X2 * Y3)) +
          ((Cf[12] * X3) + (Cf[13] * X3 * Y) + (Cf[14] * X3 * Y2) + (Cf[15] * X3 * Y3)))

counter = 0
names = ["left_uv_to_rect_x", "left_uv_to_rect_y", "right_uv_to_rect_x", "right_uv_to_rect_y"]
def print_coeffs(coeffs):
  """Print Coefficients to the console for use in other languages (this is GLSL)"""
  global counter
  flattened_coeffs = coeffs.flatten()
  num_coeffs = flattened_coeffs.shape[0]
  coeff_string = "float[] "+names[counter]+"  = float["+str(num_coeffs)+"]( "
  for i in range(num_coeffs):
    coeff_string += str(flattened_coeffs[i]) + (", " if i < num_coeffs-1 else "")
  coeff_string += ");"
  print(coeff_string)

  # Also write these coefficients to the calibration dict
  outputCalibration[names[counter]] = flattened_coeffs.tolist()

  counter += 1

def createMask(data):
  """Find the largest connected non-zero region, and mask it out"""
  # Create the "Raw Mask" from the measurements
  mask = np.clip(data[..., 1] * data[..., 2], 0, 1)

  # Find the largest contour and extract it
  contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
  maxContour = 0
  for contour in contours:
   contourSize = cv2.contourArea(contour)
   if contourSize > maxContour:
     maxContour     = contourSize
     maxContourData = contour
  
  # Create a mask from the largest contour
  mask = np.zeros_like(mask)
  mask = cv2.fillPoly(mask, [maxContourData], 1)
  return mask

def processData(data, polynomialDegree=3):
    """Fit a 2D polynomial using data, mapping from Screen UVs to either Camera Pixel or Rectilinear Coordinates"""
    # Prepare the mask of valid measurements
    mask = createMask(data)
    data *= mask[...,None]

    # Extract the Valid Measurements and arrange into a flat array
    coordinates          = np.zeros((data.shape[0], data.shape[1], 2), dtype=np.int)  
    coordinates[:, :, 0] = np.arange(0, data.shape[0])[:, None]  # Prepare the coordinates
    coordinates[:, :, 1] = np.arange(0, data.shape[1])[None, :]  # Prepare the coordinates

    # Sample the non-zero indices from the flattened coordinates and data arrays
    non_zero_indices     = np.nonzero(mask.reshape(-1))[0] # Get non-zero mask indices
    non_zero_data = data[..., 1:].reshape(-1, 2)[non_zero_indices].astype(np.float) / 255

    non_zero_pixel_coordinates = coordinates.reshape(-1, 2)[non_zero_indices].astype(np.float)

    pxc_x = non_zero_pixel_coordinates[:,0]
    pxc_y = non_zero_pixel_coordinates[:,1]

    ray_coordinates_x = []
    ray_coordinates_y = []

    for x_pix,y_pix in zip(pxc_x, pxc_y):
        x_ray = (x_pix-500)/500
        y_ray = (y_pix-500)/500
        ray_coordinates_x.append(x_ray)
        ray_coordinates_y.append(y_ray)
    ray_coordinates_x = np.array(ray_coordinates_x)
    ray_coordinates_y = np.array(ray_coordinates_y)
      # Fit the multidimensional polynomials
    x_coeffs = polyfit2d(non_zero_data[:, 1],        # "Input"  X Axis
                                             non_zero_data[:, 0],        # "Input"  Y Axis
                                             ray_coordinates_x, # "Output" X Axis
                                             np.asarray([polynomialDegree,
                                                                     polynomialDegree]))

    y_coeffs = polyfit2d(non_zero_data[:, 1],        # "Input"  X Axis
                                             non_zero_data[:, 0],        # "Input"  Y Axis
                                             ray_coordinates_y, # "Output" Y Axis
                                             np.asarray([polynomialDegree,
                                                                     polynomialDegree]))

    # Print the Calibration Coefficients
    print_coeffs(x_coeffs)
    print_coeffs(y_coeffs)

    # Reconstruct the data as if it came from the polynomial
    X, Y           = np.meshgrid(       (np.arange(0.0, 300.0)/300.0), 
                                                                1.0 - (np.arange(0.0, 533.0)/533.0))
    fitted         = np.zeros((533, 300, 3), dtype=np.float)
    fitted[..., 2] = polyval2d(X, Y, x_coeffs)
    fitted[..., 1] = polyval2d(X, Y, y_coeffs)

    return fitted

print("// Screen UV to Rectilinear Coefficients")
fitPolynomialL = processData(leftData)
fitPolynomialR = processData(rightData)

# Save the Calibrations to a .json
with open("NorthStarCalibration.json", 'w') as outfile:
  json.dump(outputCalibration, outfile)

# All this to normalize the view of the polynomial (to enhance contrast...)
polynomialToDraw = fitPolynomialL.copy()
xDraw = polynomialToDraw[:, :, 2].copy()
yDraw = polynomialToDraw[:, :, 1].copy()
cv2.normalize(polynomialToDraw[:, :, 2], dst=xDraw, alpha=0.0, beta=1.0, norm_type=cv2.NORM_MINMAX)
cv2.normalize(polynomialToDraw[:, :, 1], dst=yDraw, alpha=0.0, beta=1.0, norm_type=cv2.NORM_MINMAX)
polynomialToDraw[:, :, 2] = xDraw
polynomialToDraw[:, :, 1] = 1.0-yDraw

# Draw the cursor between the Screen Space UV's (Input) and the Rectilinear Space (Output)
warpedPos = (0,0)
def mouseMove(event, x, y, flags, param):
    '''Callback that displays a mouse cursor mapped to the original data.'''
    global dataToDraw, fitPolynomialR, warpedPos
    if event == cv2.EVENT_MOUSEMOVE:
      coord = fitPolynomialL[y, x]
      coords_3d = np.ones((1, 1, 3))
      coords_3d[0, 0, 0] = coord[2]
      coords_3d[0, 0, 1] = coord[1]
      coordd = cv2.fisheye.projectPoints(coords_3d, -cv2.Rodrigues(calibrations['R2'])[0], np.zeros((3)),
                                         calibrations['leftCameraMatrix'], calibrations['leftDistCoeffs'])[0].reshape(2)
      warpedPos = (int(coordd[0]), int(coordd[1]))

      #warpedPos = (int(coord[2] * 800), int(coord[1] * 800))
cv2.namedWindow("Polynomial Inverse"); cv2.setMouseCallback("Polynomial Inverse", mouseMove)

# Every frame, display the Raw Data and the Polynomial Inverse
key = cv2.waitKey(1)
while (not (key & 0xFF == ord('q'))):
  key = cv2.waitKey(1)

  # Draw the Cursor Warped from Screen UV Space back to Camera Space through Rectilinear Space
  dataToDraw = cv2.circle(leftData.copy(), warpedPos, 5, (255, 255, 255))

  cv2.imshow("Raw Data", dataToDraw)
  cv2.imshow("Polynomial Inverse", polynomialToDraw)
