import cv2
import math
import numpy as np
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
#defining pin numbers
output_pin1=8
output_pin2=11
input_pin=12
servo=18
lift_servo=23
direction=2
check=100
#setting GPIO modes
GPIO.setup(output_pin1, GPIO.OUT)
GPIO.setup(input_pin, GPIO.IN)
GPIO.setup(output_pin2,GPIO.OUT)
GPIO.setup(servo, GPIO.OUT)
GPIO.setup(lift_servo,GPIO.OUT)
#setting default high and low to all pins
GPIO.output(output_pin1, False)
GPIO.output(output_pin2, False)
GPIO.output(servo, False)
GPIO.output(lift_servo, False)
#techfest logo features
MIN_MATCH_COUNT=14
detector=cv2.xfeatures2d.SURF_create(5000)               #feature extractor
FLANN_INDEX_KDITREE=0                                    # not defined in opencv, hence we have to initialize
flannParam=dict(algorithm=FLANN_INDEX_KDITREE,tree=5)    
flann=cv2.FlannBasedMatcher(flannParam,{})               #feature matcher
trainImg=cv2.imread("tech_logo.jpg",0)
trainKP,trainDesc=detector.detectAndCompute(trainImg,None) # keypoint and descriptor
#lifting the block
def lifting_block()
	p = GPIO.PWM(servo, 50)
        p.start(2.5)
	p.ChangeDutyCycle(4.16)
	p = GPIO.PWM(lift_servo, 50)
	p.start(2.5)
	p.ChangeDutyCycle(7.5)
def put_block()
	q=GPIO.PWM(lift_servo,50)
	q.start(7.5)
	q.ChangeDutyCycle(2.5)
	p = GPIO.PWM(servo, 50)
        p.start(4.16)
	p.ChangeDutyCycle(2.5)
def find_blue_block()
	ret, image=cap.read()
	c=0
	hsv=cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
	mask=cv2.inRange(hsv,(100,50,50), (190,255,255))
	blur=cv2.GaussianBlur(mask,(5,5),0)
	kernel = np.ones((5,5),np.uint8)
	erosion = cv2.erode(blur,kernel,iterations = 20)
	frame, contours, hire=cv2.findContours(erosion,cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 
	for h in range (0,len(contours)):
		result= cv2.approxPolyDP(contours[h],3,1)
		for k in range (0,len(result)):
			if len(result)>=4 and len(result)<=6:
				for i in range(0, len(result)-2):
					vec1= (abs (result[i][0][0]-result[i+1][0][0]),abs(result[i][0][1]-result[i+1][0][1]) )
					vec2= (abs (result[i+2][0][0]-result[i+1][0][0]),abs(result[i+2][0][1]-result[i+1][0][1]) )
					if ((vec1[0]*vec2[0])-(vec1[1]*vec2[1]))<0.05:
						c=c+1
						if  c>=2 and c<=4:
							yes= True
							return yes 
							break
def find_direction_of_arrow()
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
		if len(result)>7 and len(result)<=15:
			for i in range(0, len(result)-2):
				vec1= (abs (result[i][0][0]-result[i+1][0][0]),abs(result[i][0][1]-result[i+1][0][1]) )
				vec2= (abs (result[i+2][0][0]-result[i+1][0][0]),abs(result[i+2][0][1]-result[i+1][0][1]) )
				cv2.line(image, (result[i][0][0],result[i][0][1]),(result[i+1][0][0], result[i+1][0][1]),(0,0,0), 5)
				if ((vec1[0]*vec2[0])-(vec1[1]*vec2[1]))==0:
					point=result[i][0]
					c=c+1
					if c>=1 and c<=2:
						centres=[]
						for h in range(0,len(contours[k])):
						moments=cv2.moments(contours[k])
						centres.append((int(moments['m10']/moments['m00']), int(moments['m01']/moments['m00'])))
						cv2.circle(arrow, centres[h], 8, (0,0,255), -1)
						if point[0]>centres[0][0]:
							a=0          #right
						else:
							a=1	     #left
						return a
						break
def detecting_tf_block()
	    ret, QueryImgBGR=cam.read()
	    QueryImg=cv2.cvtColor(QueryImgBGR,cv2.COLOR_BGR2GRAY)
	    queryKP,queryDesc=detector.detectAndCompute(QueryImg,None)
	    matches=flann.knnMatch(queryDesc,trainDesc,k=2)

	    goodMatch=[]
	    for m,n in matches:
		if(m.distance<0.75*n.distance):
		    goodMatch.append(m)
			  # m contains all the keypoints	
		  	  # we only need keypoints that are filtered
	    if(len(goodMatch)>=MIN_MATCH_COUNT):
		tp=[]
		qp=[]
		for m in goodMatch:
		    tp.append(trainKP[m.trainIdx].pt)
		    qp.append(queryKP[m.queryIdx].pt)

		tp,qp=np.float32((tp,qp))       # converting python list to numpy array
		H,status=cv2.findHomography(tp,qp,cv2.RANSAC,3.0)   # to get the orientation of incoming query image
		h,w=trainImg.shape   
		trainBorder=np.float32([[[0,0],[0,h-1],[w-1,h-1],[w-1,0]]]) # points in anti-clockwise order
		queryBorder=cv2.perspectiveTransform(trainBorder,H)      
		yes= True
	    else:
		yes=False
	    return yes
#main_code
while GPIO.input(input_pin)==1:
	blue=find_blue_block()
	if blue== True:
		GPIO.output(output_pin1, True)
		while GPIO.input(input_pin)==0:
			lifting_block()
			GPIO.output(output_pin1, False)
			break
		while GPIO.input(input_pin)==1:
			direction=find_direction_of_arrow()
			if direction==0: #right
				GPIO.output(output_pin1, True)
			if direction==1: #left
				GPIO.output(output_pin2, True)
		while GPIO.input(input_pin)==0:
			put_block()
		       	if direction==0:     #right
				GPIO.output(output_pin1,False)
			if direction==1:
				GPIO.output(output_pin2, False)
			break
		while GPIO.input(input_pin)==1:
			lifting_block()	
			GPIO.output(output_pin2, True)
			break
		while GPIO.input(input_pin)==0:	
			put_block()
			GPIO.output(output_pin2, False)
			break
	if blue== False:
		GPIO.output(output_pin2, True)
		while GPIO.input(input_pin)==0:
			lifting_block()
			GPIO.output(output_pin1, False)
			break
		while GPIO.input(input_pin)==1:
			direction=find_direction_of_arrow()
			if direction==0: #right
				GPIO.output(output_pin1, True)
			if direction==1: #left
				GPIO.output(output_pin2, True)
		while GPIO.input(input_pin)==0:
			put_block()
			if direction==0:     #right
				GPIO.output(output_pin1,False)
			if direction==1:
				GPIO.output(output_pin2, False)
			break
		while GPIO.input(input_pin)==1:
			lifting_block()
			GPIO.output(output_pin2, True)
			break
		while GPIO.input(input_pin)==0:
			put_block()
			GPIO.output(output_pin2,False)
			break
#Now run2
#has to detect techfest logo
#goes to coordinates of photo point
while GPIO.input(input_pin)==1:
	sim= detecting_tf_block()
	if sim== False:
		GPIO.output(output_pin2, True)
		break
	else :
		GPIO.output(output_pin1, True)
		check=0
		while GPIO.input(input_pin)==0:
			lifting_block()
			GPIO.output(output_pin1, False)
			break
		while GPIO.input(input_pin)==1 and check=100:
			sim= detecting_tf_block()
			if sim ==False:
				GPIO.output(output_pin2, True)
				break
			else :
				GPIO.output(output_pin1, True)
				check=0
				while GPIO.input(input_pin)==0:
					lifting_block()
					GPIO.output(output_pin1, False)
					break
				while GPIO.input(input_pin)==1 and check=100:
				sim= detecting_tf_block()
				if sim==False:
					GPIO.output(output_pin2, True)
					break
				else :
					GPIO.output(output_pin1, True)
					check=0
					while GPIO.input(input_pin)==0:
						lifting_block()
						GPIO.output(output_pin1, False)
						break
	while GPIO.input(input_pin)==1:
		put_block()
		GPIO.output(output_pin1, False)
	 	break  
			
