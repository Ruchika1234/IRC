import numpy as np
import cv2
import cmath
import math

arrow=np.zeros((500,500,3), np.uint8)
arrow[arrow<30]=254
ppt=[]
points=np.array([[200,30], [100,30], [100,20], [75,50], [100,80], [100,70], [200,70]], np.int32)
max=500
min=0
cv2.fillConvexPoly(arrow, points, (0,0,255), 4, 0)
hsv=cv2.cvtColor(arrow, cv2.COLOR_BGR2HSV)
lower = (0,0,0)
upper = (0,0,255)
max=0
min=500,500
mask= cv2.inRange(arrow, lower, upper)
image, contours, hierarchy= cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
for i in range (0,len(contours)):
	result= cv2.approxPolyDP(contours[i],3,1)
	if len(result)==7:
		cv2.drawContours(arrow, contours[i], -1,(0,255,0),3)
		print(len(result))
		for i in range(0,len(result)):
			print(result[i])
		for i in range (0, len(points)):
			print(points[i])
		for i in range(0, len(result)-2):
			vec1= (abs (result[i][0][0]-result[i+1][0][0]),abs(result[i][0][1]-result[i+1][0][1]) )
			vec2= (abs (result[i+2][0][0]-result[i+1][0][0]),abs(result[i+2][0][1]-result[i+1][0][1]) )
			len1 = math.hypot(vec1[0],vec1[1])
    			len2 = math.hypot(vec2[0],vec2[1])
			if vec1[0]*vec2[0]-vec1[1]*vec2[1]!=0:
				print(vec1[0]*vec2[0]-vec1[1]*vec2[1])
				point=result[i][0]
 				print(point)
				print(i)
				break
		centres=[]
		for i in range(len(contours)):
			moments=cv2.moments(contours[i])
			centres.append((int(moments['m10']/moments['m00']), int(moments['m01']/moments['m00'])))
			cv2.circle(arrow, centres[i], 3, (0,0,0), -1)
                	print(centres[i])
if point[0]>centres[0][0]:
	print("right")
else:
	print("left")	
