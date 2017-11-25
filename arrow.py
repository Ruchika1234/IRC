import numpy as np
import cv2

arrow=np.zeros((500,500,3), np.uint8)
arrow[arrow<30]=254
points=np.array([[100,30], [200,30], [200,20], [225,50], [200,80], [200,70], [100,70]], np.int32)
cv2.fillConvexPoly(arrow, points, (0,0,255), 4, 0)
hsv=cv2.cvtColor(arrow, cv2.COLOR_BGR2HSV)
lower = (0,0,0)
upper = (0,0,255)
mask= cv2.inRange(arrow, lower, upper)
image, contours, hierarchy= cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
cv2.drawContours(arrow, contours, -1,(0,255,0),3)
centres=[]
for i in range(len(contours)):
    moments=cv2.moments(contours[i])
    centres.append((int(moments['m10']/moments['m00']), int(moments['m01']/moments['m00'])))
    cv2.circle(arrow, centres[i], 3, (0,0,0), -1)#
print(len(contours))
if int(moments['m10']/moments['m00'])> 225:
    print("left")
else:
    print("right")
cv2.imshow("Image", arrow)
cv2.imshow("hsv", hsv)
cv2.waitKey(0)
