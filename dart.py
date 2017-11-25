import numpy as np
import cv2
from math import atan

dart= cv2.imread("circle.jpg")
gray=cv2.cvtColor(dart, cv2.COLOR_BGR2GRAY)
blur=cv2.GaussianBlur(gray,(5,5),0)
image, contours, hierarchy= cv2.findContours(blur, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
print(len(contours))
c=0
h=0
ref=(0,500)
ang=0
for i in range(0,len(contours)):
    cv2.drawContours(dart, contours, i,(0,255,0),3)
    if len(contours[i])!=4:
              (center,size,angle)=cv2.fitEllipse(contours[i])
              center=(int(center[0]), int(center[1]))
              size=(int(size[0]), int(size[1]))
              cv2.ellipse(dart,center,size,0.0,0.0,360, (0,0,255),-1)
              c=c+1
              h=h+center[0]
              ang=ang+atan(center[0]/abs(center[1]-ref[1]))
print(h/c)
print(ang/c)
cv2.imshow("win", dart)
cv2.waitKey(0)

                     
