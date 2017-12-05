import numpy as np
import cv2 
import math

cap=cv2.VideoCapture(0)
while True:
	ret, arrow=cap.read()
	hsv=cv2.cvtColor(arrow, cv2.COLOR_BGR2HSV)
	lower = (0,0,0)
	upper = (180,255,75)
	c=0
	mask= cv2.inRange(hsv, lower, upper)
	blur=cv2.GaussianBlur(mask,(5,5),0) 
	crop_img = blur[200:400, 100:300]
	image, contours, hierarchy= cv2.findContours(crop_img, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
	for k in range (0,len(contours)):
		result= cv2.approxPolyDP(contours[k],3,1)
		print(len(result))
		for j in range (0,len(result)):
			cv2.circle(arrow, (result[j][0][0],result[j][0][1]), 5, (0,0,0), -1)
		if len(result)>7 and len(result)<=15:
			 cv2.drawContours(arrow, contours[k], -1,(0,0,255),3)
			 for i in range(0, len(result)-2):
				vec1= (abs (result[i][0][0]-result[i+1][0][0]),abs(result[i][0][1]-result[i+1][0][1]) )
				vec2= (abs (result[i+2][0][0]-result[i+1][0][0]),abs(result[i+2][0][1]-result[i+1][0][1]) )
				cv2.line(image, (result[i][0][0],result[i][0][1]),(result[i+1][0][0], result[i+1][0][1]),(0,0,0), 5)
				if ((vec1[0]*vec2[0])-(vec1[1]*vec2[1]))==0:
					point=result[i][0]
					c=c+1
					print("c", c)
	 		 if c>=1 and c<=2:
				print("yes")
				cv2.drawContours(arrow, contours[k], -1,(255,255,0),3)
				centres=[]
				for h in range(0,len(contours[k])):
					moments=cv2.moments(contours[k])
					centres.append((int(moments['m10']/moments['m00']), int(moments['m01']/moments['m00'])))
					cv2.circle(arrow, centres[h], 8, (0,0,255), -1)
					if point[0]>centres[0][0]:
						print("right")
					else:
						print("left")	
	cv2.imshow("win",crop_img )
	cv2.imshow("win1", arrow)
	if cv2.waitKey(1) & 0xFF == ord('q'):
        	break		
	
				
	
	
