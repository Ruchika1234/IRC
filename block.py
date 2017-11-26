import numpy as np
import cv2

image=cv2.imread("blocks.jpg")
hsv=cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
mask=cv2.inRange(hsv, (110,50,50), (130,255,255))
blur=cv2.GaussianBlur(mask,(5,5),0)
frame, contours, hire=cv2.findContours(blur,cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 
for i in range (0, len(contours)):
		result= cv2.approxPolyDP(contours[i],3,1)
		if len(result)==4:
			for i in range (0,len(result)-1):
				cv2.line(image, (result[i][0][0],result[i][0][1]),(result[i+1][0][0], result[i+1][0][1]),(0,255,0), 5)
mask2=cv2.inRange(hsv,(0,100,100), (10,255,255))
blur2=cv2.GaussianBlur(mask2,(5,5),0)
frame1, contours1, hire1=cv2.findContours(blur2,cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 
for i in range (0, len(contours1)):
		result1= cv2.approxPolyDP(contours1[i],3,1)
		if len(result1)==4:
			for i in range (0,len(result1) -1):
				cv2.line(image, (result1[i][0][0],result1[i][0][1]),(result1[i+1][0][0], result1[i+1][0][1]),(255,255,0), 5)
cv2.imshow("image", image)
cv2.imshow("win", mask)
cv2.imshow("win2", mask2)
cv2.waitKey(0)

