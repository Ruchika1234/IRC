import cv2
import numpy as np

similarity=0
image=cv2.imread("techfest.jpg")
surf = cv2.xfeatures2d.SURF_create(5000)
kp, des = surf.detectAndCompute(image,None)
img2 = cv2.drawKeypoints(image,kp,None,(255,0,0),4)
image1=cv2.imread("techfest_blues.jpg")
surf = cv2.xfeatures2d.SURF_create(5000)
kp1, des1 = surf.detectAndCompute(image1,None)
img21 = cv2.drawKeypoints(image1,kp1,None,(255,0,0),4)
#for i in range(0,len(kp)):
#	print(kp[i])
#	print(kp1[i])
#	if kp[i]==kp1[i]:
#		similarity =similarity+1
#		print("hi")
#if similarity>10:
#	print(similarity)
#	print("similar")
if len(des)<len(des1):
	c=len(des)
else:
	c=len(des1)
if len(des[1])<len(des1[1]):
	b=len(des[1])
else:
	b=len(des1[1])
for i in range (0,c):
	for j in range (0,b):
		if(des[i][j]==des1[i][j]):
			similarity =similarity+1
print(similarity)
if similarity >400:
	print("same")
cv2.imshow("win",img2)
cv2.imshow("win1", img21)
cv2.waitKey(0)
