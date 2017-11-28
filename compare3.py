import cv2
import numpy as np

images=[]
img1=cv2.imread("techfest.jpg")
img2=cv2.imread("curve.jpg")
img3=cv2.imread("superman.jpg")
images.append(img1)
images.append(img2)
images.append(img3)
for i in range (0, len(images)):
	gray=cv2.cvtColor(images[i], cv2.COLOR_BGR2HSV)
	thresh=cv2.inRange(gray,(0,0,0), (255,255,0))
	blur=cv2.GaussianBlur(thresh, (5,5),0)
	frame, contours, hire=cv2.findContours(blur, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
	cv2.drawContours(blur, contours, -1,(0,255,0),2)
	canny=cv2.Canny(gray,100,200,apertureSize = 3)
	print(len(contours))
	lines = cv2.HoughLines(canny,1,np.pi/180,200)
	print(len(lines))
	if len(lines)==4 and len(contours)==3:
		cv2.imshow("image", images[i])
		cv2.waitKey(0)
		


