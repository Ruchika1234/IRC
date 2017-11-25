import numpy as np
import cv2

image=cv2.imread("all.jpg")
hsv= cv2.cvtColor(image, cv2.COLOR_BGR2HSV) 
low=(0,0,0)
upper=(255,0,0)
c=0
d=0
cen=(0,0)
gray=cv2.inRange(hsv, low, upper)
blur=cv2.GaussianBlur(gray,(5,5),0)
frame, contours, hire=cv2.findContours(blur,cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
print(len(contours))
#cv2.drawContours(image, contours,1, (255,0,0), 3)
#cv2.drawContours(image, contours,2, (0,255,0), 3)
#cv2.drawContours(image, contours,3, (0,0,255), 3)
#cv2.drawContours(image, contours,4, (255,255,0), 3)
#cv2.drawContours(image, contours,5, (255,0,255), 3)
#cv2.drawContours(image, contours,6, (0,255,255), 3)
#cv2.drawContours(image, contours,7,(255,255,255), 3)
#cv2.drawContours(image, contours,8, (255,0,56), 3)
#cv2.drawContours(image, contours,9, (0,120,167), 3)
for i in range (0, len(contours)):
	if len(contours[i])!=4:
		(center, size, angle)=cv2.fitEllipse(contours[i])
		center=(int(center[0]), int(center[1]))		
		size=(int(size[0]), int (size[1]))
                print(center)
		if (cen[0]!=center[0]):
			cen=center
			c=c+1
			print(cen)
		else:
			d=d+1
			cen=center
			if d==1:
				cv2.ellipse(image,cen,size,0.0,0.0,360, (0,0,255),-1)
				break
				print(cen)
print(d)
cv2.imshow("win", image)
cv2.imshow("win1", gray)
cv2.waitKey(0)




