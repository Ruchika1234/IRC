import cv2
import numpy as np

cap=cv2.VideoCapture(0)
f=0
cen=(0,0)
dart_center=(0,0)
while True:
	ret, image=cap.read()
	hsv= cv2.cvtColor(image, cv2.COLOR_BGR2HSV) 
	low=(0,0,0)
	upper=(180,255,80)
	gray=cv2.inRange(hsv, low, upper)
	blur=cv2.GaussianBlur(gray,(5,5),0) 
	frame, contours, hire=cv2.findContours(blur,cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
	cv2.drawContours(image, contours,-1, (255,0,0), 2)
	for i in range (0, len(contours)):
		if len(contours[i])!=4:
			(center, size, angle)=cv2.fitEllipse(contours[i])
			center=(int(center[0]), int(center[1]))		
			size=(int(size[0]), int (size[1]))
			if (cen[0]!=center[0]):
				cen=center
			else:
				cen=center
				if dart_center[0]!=cen[0]:
					dart_center=cen
				elif dart_center[0]-cen[0]<4:
					f=f+1
				if  f==1:
					cv2.ellipse(image,cen,size,0.0,0.0,360, (0,0,255),5)
					print(dart_center)
					break
	cv2.imshow("win", gray)
	cv2.imshow("win1", image)
	if cv2.waitKey(1) & 0xFF == ord('q'):
        	break	
