import numpy as np
import cv2
from math import atan
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

GPIO.setup(12, GPIO.OUT)

p = GPIO.PWM(12, 50)
p.start(2.5)
time.sleep(1)
cap=cv2.VideoCapture(0)
while(True):
    ret, dart= cap.read()
    gray=cv2.cvtColor(dart, cv2.COLOR_BGR2GRAY)
    blur=cv2.GaussianBlur(gray,(5,5),0)
    image, contours, hierarchy= cv2.findContours(blur, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    print(len(contours))
    c=0
    h=0
    ref=(0,500)
    ang=0
    for i in range(0,len(contours)):
        cv2.drawContours(dart, contours, i,(0,255,0),3)
        if len(contours[i])!=4:
              (center,size,angle)=cv2.fitEllipse(contours[i])
              center=(int(center[0]), int(center[1]))
              size=(int(size[0]), int(size[1]))
              cv2.ellipse(dart,center,size,0.0,0.0,360, (0,0,255),-1)
              c=c+1
              h=h+center[0]
              ang=ang+atan(center[0]/abs(center[1]-ref[1]))
              cv2.imshow("frame",dart)
print(h/c)
print(ang/c)
a=ang/c
p.ChangeDutyCycle(2.5+a/18)