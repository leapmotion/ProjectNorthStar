from __future__ import print_function
import pyrealsense2 as rs
import cv2
import numpy as np
import threading
import time

class intelCamThread(threading.Thread):
    '''A thread that handles retrieving imagery from the Intel T265'''
    def __init__(self, frame_callback, source = 0, frameWidth = 384, timeout = 40.0):
        '''Initialize Rigel Image Capture'''
        threading.Thread.__init__(self)
        self.frame_callback = frame_callback
        self.frame = None
        self.newFrame = False
        self.frameWidth = frameWidth
        self.deadmansSwitch = time.time()
        self.timeout = timeout

        self.paused = False

        self.frame_mutex = threading.Lock()
        self.frame_data = {"left"         : None,
                           "right"        : None,
                           "timestamp_ms" : None,
                           "newFrame"     : False}

        # Declare RealSense pipeline, encapsulating the actual device and sensors
        self.pipe = rs.pipeline()
        # Build config object and stream everything
        self.cfg = rs.config()
        # Start streaming with our callback
        self.pipe.start(self.cfg, self.callback)

        self.success = False
        try:
            # Retrieve the stream and intrinsic properties for both cameras
            profiles = self.pipe.get_active_profile() 
            streams =    {"left"  : profiles.get_stream(rs.stream.fisheye, 1).as_video_stream_profile(),
                          "right" : profiles.get_stream(rs.stream.fisheye, 2).as_video_stream_profile()}
            intrinsics = {"left"  : streams["left"] .get_intrinsics(),
                          "right" : streams["right"].get_intrinsics()}

            # Print calibration information about both cameras
            #print("Left camera:",  intrinsics["left"])
            #print("Right camera:", intrinsics["right"])

            # Translate the intrinsics from librealsense into OpenCV
            K_left  = self.camera_matrix(intrinsics["left"])
            D_left  = self.fisheye_distortion(intrinsics["left"])
            K_right = self.camera_matrix(intrinsics["right"])
            D_right = self.fisheye_distortion(intrinsics["right"])
            (width, height) = (intrinsics["left"].width, intrinsics["left"].height)

            # Get the relative extrinsics between the left and right camera
            (R, T) = self.get_extrinsics(streams["left"], streams["right"])

            # We set the left rotation to identity and the right rotation
            # the rotation between the cameras
            R_left = np.eye(3)
            R_right = R

            # Create an undistortion map for the left and right camera which applies the
            # stereographification and undoes the camera distortion. This only has to be done once
            m1type = cv2.CV_32FC1
            (lm1, lm2) = self.constructStereographicWarpMap(self.frameWidth, K_left,  D_left,  R_left)
            (rm1, rm2) = self.constructStereographicWarpMap(self.frameWidth, K_right, D_right, R_right)
            self.undistort_stereographify = {"left" : (lm1, lm2), "right" : (rm1, rm2)}
            self.success = True
        finally:
            if(not self.success):
                self.kill()

    def run(self):
        print("Beginning Video Capture Thread! (at %f)" % time.time())
        while(True):
            now = time.time()
            if now - self.deadmansSwitch > self.timeout:
                print(f"{now} - {self.deadmansSwitch} > {self.timeout}")
                break
            # Check if the camera has acquired any new frames
            self.frame_mutex.acquire()
            valid = ((self.frame_data["timestamp_ms"] is not None) and self.frame_data["newFrame"])
            self.frame_mutex.release()

            # If there is a new frame ready to process
            if (valid):
                # Hold the mutex only long enough to copy the stereo frames and set the dirty flag
                self.frame_mutex.acquire()
                frame_copy = {"left"  : self.frame_data["left" ].copy(),
                              "right" : self.frame_data["right"].copy()}
                self.frame_data["newFrame"] = False
                self.frame_mutex.release()

                # leftRightImage = np.empty((frame_copy["left"].shape[0],
                #                            frame_copy["left"].shape[1], 2), dtype=np.uint8)
                # leftRightImage[:,:,0] = np.transpose(frame_copy["left"])
                # leftRightImage[:,:,1] = np.transpose(frame_copy["right"])

                # captureGraycodes.py expects it like this:
                combinedImage = np.hstack([frame_copy["left"], frame_copy["right"]])

                if (not self.paused):
                    self.frame = combinedImage
                    self.newFrame = True

                self.frame_callback(self.frame)
                time.sleep(0.1) # Sleep to allow the camera callback thread to butt in!
            else:
                time.sleep(0.1) # Sleep to allow the camera callback thread to butt in!

        print("Exiting Video Capture Thread! (at %f)" % time.time())
        if(self.success):
            self.pipe.stop()

    def read(self):
        '''Attempts to retrieve the latest rigel image; also resets the dead-man's switch on the capture thread'''
        self.deadmansSwitch = time.time()
        newFrame = self.newFrame
        self.newFrame = False
        return newFrame, self.frame

    def set(self, param, value):
        '''NOP.'''
        return 0

    def setGain(self, gain):
        '''NOP. \n(param, gain) -> (ret)'''
        return 0

    def setPaused(self, paused):
        '''Pauses the camera image'''
        self.paused = paused

    def get(self, param):
        '''NOP. \n(param) -> (int)'''
        return 0

    def kill(self):
        '''Ends the video capture thread'''
        self.deadmansSwitch = self.deadmansSwitch - self.timeout

    def stereographic_to_3D(self, points):
        '''Put stereographic points into 3D space (Z is Depth for OpenCV!)'''
        fmult    = 1/(1 + (points[:,0]*points[:,0]) + (points[:,1]*points[:,1]))
        zCoord   =  (-1 + (points[:,0]*points[:,0]) + (points[:,1]*points[:,1]))
        spoints  = np.concatenate(((-2*points[:,0]*fmult)[:, None], 
                                   (-2*points[:,1]*fmult)[:, None], 
                                           (zCoord*fmult)[:, None]), axis=1)
        return spoints / np.linalg.norm(spoints, axis=-1)[:, None]

    def rectilinear_to_3D(self, points):
        '''Put rectilinear points into 3D space (Z is Depth for OpenCV!)'''
        return np.stack((points[:,0], 
                         points[:,1], 
                 np.ones(points.shape[0])), axis=1).astype(np.float32)

    def constructStereographicWarpMap(self, mapWidth, cameraMatrix, distCoeffs, rVec):
        # 1) Calculate Stereographic coords
        #x = np.linspace(-1, 1, mapWidth)
        #stereographicCoords = np.stack(np.meshgrid(x, x), axis=2).reshape(-1, 2)
        stereographicCoords = np.empty((mapWidth*mapWidth, 2), np.float32)
        for x in range(mapWidth):
            for y in range(mapWidth):
                stereographicCoords[(y*mapWidth)+x] = np.asarray([((x/mapWidth)*2)-1,
                                                                  ((y/mapWidth)*2)-1])

        # 2) Convert Stereographic Coordinates to 3D
        pointsToProject = self.stereographic_to_3D(stereographicCoords)#self.rectilinear_to_3D(stereographicCoords)#

        # 3) Project 3D coords to Pixel coords
        pointsToProject = np.reshape(pointsToProject, (mapWidth*mapWidth, 1, 3))
        rVec = np.reshape(rVec, (3, 3))
        pixelPoints, garbage1 = cv2.fisheye.projectPoints(pointsToProject, np.zeros((3)), np.zeros((3)), cameraMatrix, distCoeffs)

        # 4) Write Pixel coords to warp maps
        reshapedPixels = pixelPoints.reshape(mapWidth, mapWidth, 2)
        return (reshapedPixels[:,:,0], reshapedPixels[:,:,1])

    def get_extrinsics(self, src, dst):
        """Returns R, T transform from src to dst"""
        extrinsics = src.get_extrinsics_to(dst)
        R = np.reshape(extrinsics.rotation, [3,3]).T
        T = np.array(extrinsics.translation)
        return (R, T)

    def camera_matrix(self, intrinsics):
        """Returns a camera matrix K from librealsense intrinsics"""
        return np.array([[intrinsics.fx,             0, intrinsics.ppx],
                         [            0, intrinsics.fy, intrinsics.ppy],
                         [            0,             0,              1]])

    def fisheye_distortion(self, intrinsics):
        """Returns the fisheye distortion from librealsense intrinsics"""
        return np.array(intrinsics.coeffs[:4])

    def callback(self, frame):
        """
        This callback is called on a separate thread, so we must use a mutex
        to ensure that data is synchronized properly. We should also be
        careful not to do much work on this thread to avoid data backing up in the
        callback queue.
        """
        if frame.is_frameset():
            frameset = frame.as_frameset()
            f1 = frameset.get_fisheye_frame(1).as_video_frame()
            f2 = frameset.get_fisheye_frame(2).as_video_frame()
            left_data = np.asanyarray(f1.get_data())
            right_data = np.asanyarray(f2.get_data())
            ts = frameset.get_timestamp()
            self.frame_mutex.acquire()
            self.frame_data["left"]  = left_data
            self.frame_data["right"] = right_data
            self.frame_data["timestamp_ms"] = ts
            self.frame_data["newFrame"] = True
            self.frame_mutex.release()