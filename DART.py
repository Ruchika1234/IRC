import cv2
import numpy as np
import math
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
servo_rotate=12
servo_height=23
GPIO.setup(servo_rotate, GPIO.OUT)
GPIO.setup(servo_height, GPIO.OUT)
p = GPIO.PWM(servo_rotate, 50)
p.start(7.5)
q = GPIO.PWM(servo_height,50)
q.start(7.5)
cap=cv2.VideoCapture(0)
cap1=cv2.VideoCapture(0)
cen=(0,0)
dart_center=(0,0)
cen1=(0,0)
laser_center=(0,0)
for i in range (0,200):
	ret, image=cap.read()
	hsv= cv2.cvtColor(image, cv2.COLOR_BGR2HSV) 
	low=(0,0,0)
	upper=(180,255,80)
	f=0
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
					break		
for i in range(0, 200):
		hsv1=cv2.cvtColor(image,cv2.COLOR_BGR2HSV)
		low1=(0,0,0)
		upper1=(170,255,255)
		f1=0
		gray1=cv2.inRange(hsv1, low1,upper1)
		blur1=cv2.GaussianBlur(gray1,(5,5),0)
		frame1, contours1, hire1=cv2.findContours(blur1,cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
		for i in range( len(contours1)):
		   if len(contours1[i])>4:
		 	(center1, size1, angle1)=cv2.fitEllipse(contours1[i])
		 	center1=(int(center1[0]), int(center1[1]))
			size1=(int(size1[0]), int (size1[1]))
			if (cen1[0]!=center1[0]):
				cen1=center1
			else:
				cen1=center1
				if laser_center[0]!=cen1[0]:
					laser_center=cen1
				elif laser_center[0]-cen[0]<2 :
					f1=f1+1
				if  f1==1:
					break	
height=abs(laser_center[1]-dart_center[1])
angle_servo=math.atan(abs(laser_center[0]-dart_center[0])/height)
angle_lift=math.atan(height/1720)
p.ChangeDutyCycle(7.5+angle_servo*3.1840)
q.ChangeDutyCycle(7.5+angle_lift*3.1840)
