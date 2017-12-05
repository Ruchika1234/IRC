import numpy as np
import cv2

cap=cv2.VideoCapture(0)
while True:
	ret, image=cap.read()
	c=0
	hsv=cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
	mask=cv2.inRange(hsv,(100,50,50), (190,255,255))
	blur=cv2.GaussianBlur(mask,(5,5),0)
	kernel = np.ones((5,5),np.uint8)
	erosion = cv2.erode(blur,kernel,iterations = 20)
	#opening = cv2.morphologyEx(blur, cv2.MORPH_OPEN, kernel)
	#closing = cv2.morphologyEx(blur, cv2.MORPH_CLOSE, kernel)
	frame, contours, hire=cv2.findContours(erosion,cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 
	cv2.drawContours(image, contours, -1,(0,0,255),3)
	for h in range (0,len(contours)):
		result= cv2.approxPolyDP(contours[h],3,1)
	#for k in range (1, len(contours)):
	#		if max<len(contours[k]):
	#			max=len(contours[k])
	 #                       result= cv2.approxPolyDP(contours[k],3,1)
		for k in range (0,len(result)):
		#				print("max,", max)
			cv2.circle(image, (result[k][0][0],result[k][0][1]), 5, (0,0,0), -1)
			print(len(result))
			if len(result)>=4 and len(result)<=6:
					for i in range(0, len(result)-2):
						vec1= (abs (result[i][0][0]-result[i+1][0][0]),abs(result[i][0][1]-result[i+1][0][1]) )
						vec2= (abs (result[i+2][0][0]-result[i+1][0][0]),abs(result[i+2][0][1]-result[i+1][0][1]) )
						if ((vec1[0]*vec2[0])-(vec1[1]*vec2[1]))<0.05:
							c=c+1
							print("c", c)
							if  c>=2 and c<=4:
								cv2.drawContours(image, contours[h], -1,(0,255,0),3)
								break
								#c#v2.line(image, (result[i][0][0],result[i][0][1]),(result[i+1][0][0], result[i+1][0][1]),(0,0,0), 5)
	cv2.imshow("image", image)
	cv2.imshow("win", erosion)
	#cv2.imshow("win1", closing)
	if cv2.waitKey(1) & 0xFF == ord('q'):
        	break				
