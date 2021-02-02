import numpy as np
import cv2
 
cap = cv2.VideoCapture(1)
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)
cap.set(cv2.CAP_PROP_EXPOSURE, -3.0)
 
while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
 
    # Our operations on the frame come here
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
 
    # Display the resulting frame
    cv2.imshow('frame',gray)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
 
# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
