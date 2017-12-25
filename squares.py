import cv2
import numpy as np
import math 

def angle(pt1, pt2,pt0):
	dx1=pt1[0]-pt0[0]
	dy1=pt1[1]-pt0[1]
	dx2=pt2[0]-pt0[0]
	dy2=pt2[1]-pt0[1]
	return  (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10)
thresh = 50
N = 10
def find_squares(image):
	gray0=cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
	pyr=cv2.pyrDown(gray0)
	timg=cv2.pyrUp(pyr)
	for c in range (0,3):
		cv2.mixChannels([timg], [gray0], [0,0])
		for l in range (0,N):
			if l == 0: 
			 	gray=cv2.Canny(gray0,thresh,100)
			 	kernel = np.ones((5,5),np.uint8)
				dilation = cv2.dilate(gray,kernel,iterations = 2)
			else:
					gray = gray0 >= (l+1)*255/N; 
			ret, contours, hire=cv2.findContours(dilation,cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
			for i in range (0,len(contours)):
				approx= cv2.approxPolyDP(contours[i],3,1)
				convex = cv2.isContourConvex(contours[i])
				if len(approx)==4 and math.fabs(cv2.contourArea(contours[i])) and convex== True:
					print("square")
					maxCosine = 0
					for j in range(2,5):
						cosine = math.fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        			if (maxCosine>cosine):
                        				maxCosine=cosine
                       				if maxCosine < 0.3:
                        				squares.append(approx)


cam=cv2.VideoCapture(0)
while True:
	ret, frame=cam.read()
	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	mask=cv2.inRange(gray,(100,50,50),(190,255,255))
	dst=cv2.cvtColor(mask,cv2.COLOR_GRAY2BGR);
	#blur=cv2.GaussianBlur(mask, (5, 5), 0)
	find_squares(dst)
	if cv2.waitKey(10)==ord('q'):         
			break               
