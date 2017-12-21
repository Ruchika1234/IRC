#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <math.h>
//#include <string.h>
#include "opencv2/core.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/nonfree/xfeatures2d.hpp"
//#include "GPIOClass.h"
//#include <unistd.h>
//#include <errno.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <wiringPi.h>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;
// global variables
int direction=2;
// square detect
VideoCapture cam(0);
Mat frame;
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

static bool find_blue_Squares()
{   
	 cam.read(frame);
   Mat image=frame.clone();
   bool value = false;
   Mat mask,image1;
    inRange(image, Scalar(190,20,20), Scalar(255,100,100),mask);
    cvtColor(mask, image1,CV_GRAY2BGR);
   Mat gray0(image1.size(), CV_8U), gray;   
    
    vector<vector<Point> > contours;
    gray0 = image1;	
    
       
    Canny(gray0, gray, 0, 50, 5);               
    dilate(gray, gray, Mat(), Point(-1,-1)); 
    imshow("dilate",gray); 
         
    findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    vector<Point> approx;
    for( size_t i = 0; i < contours.size(); i++ )
    {                
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
        if( approx.size() == 4 && fabs(contourArea(Mat(approx))) > 1600 &&   isContourConvex(Mat(approx)) )
        {
            double maxCosine = 0;

            for( int j = 2; j < 5; j++ )
            {                        
                double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                maxCosine = MAX(maxCosine, cosine);
            }
            if( maxCosine < 0.3 )
            {
                value = true;
		    }
        }
    }  
    return value;  
  }


// arrow detect
static int arrowDetect(const Mat& img)
{
		char value = ' ';
    cam.read(frame);
    Mat image=frame.clone();
		Mat hsv,mask,dst;
    double max_x,avg_x,min_x,max_contour=0;
    cvtColor(image, hsv, CV_BGR2HSV);
    inRange(hsv,Scalar(0,0,0), Scalar(180,255,80),mask);
    GaussianBlur( mask, dst, Size( 5, 5 ), 0, 0 );
    vector<vector<Point> > contours;
    vector<Vec4i> hire;   
    findContours( dst, contours, hire, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    vector<vector<Point> > approx;
          for( int h=1;h<contours.size();++h)
          {
            if (contourArea(contours[max_contour])<contourArea(contours[h]))
            {
              max_contour=h;
            }
          }
      approx.resize(contours[max_contour].size());
        //approxPolyDP(contours[max_contour], approx, 3, true);
      
      for (int d=0;d<approx.size();++d)
      {
        approxPolyDP(contours[max_contour], approx[d], 3, true);
      }
      for (int d=0;d<approx.size();++d)
      {
        if (approx[d].size()>=7 && approx[d].size()<=15)
        {
          drawContours(image, contours, max_contour,(0,255,0),1, 8, hire, 0, Point() );
          max_x=approx[d][0].x;
          for (int g=1;g<approx[d].size();++g)
          {
      //cout<<approx[d][g].x<<endl;
      circle(image, Point(approx[d][g].x,approx[d][g].y), 6,(0,255,255), 1,8,0);
            if (max_x<approx[d][g].x)
            {
              max_x=approx[d][g].x;
            }
          }
          min_x=approx[d][0].x;
          for (int g=1;g<approx[d].size();++g)
          {
            if (min_x>approx[d][g].x)
            {
              min_x=approx[d][g].x;
            }
          }
          avg_x=(max_x+min_x)/2;
          vector<Moments> mu(contours[max_contour].size() );
      for(int i=0; i<contours[max_contour].size(); ++i)
      {
      mu[i]=moments(contours[max_contour], false);
      }
      vector<Point2f> mc(contours[max_contour].size() );
      for(int i=0;i<contours[max_contour].size(); ++i)
      {
      mc[i]=Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
      }
      for (int h=0;h<mc.size();++h)
      {
      if(mc[h].x<avg_x)
      {
        direction=0;//right
      }
      else if (mc[h]>avg.x)
      {
        direction=1;//left
      
      }
      else if (mc[h]==avg.x)
      {}
    }
  }
}
return direction;
}


// logo detect

static bool logoDetect()
{
    cam.read(frame);
    Mat image=frame.clone();
    Mat objectMat = imread("Techfest.jpg");
    int hessianValue = 1600;
    int match =30;
    bool value = false;
   
    Mat sceneMat = image.clone();
   
    vector< KeyPoint > keypointsO;
    vector< KeyPoint > keypointsS;    
 
    Mat descriptors_object, descriptors_scene;      

     Ptr<SURF> detector = SURF::create( hessianValue );
        detector->detect(sceneMat,keypointsS);
    if(keypointsS.size() < 5) return false; 
        detector->detect(objectMat,keypointsO);
    if(keypointsO.size() < 5) return false;
 
 
  Ptr<SURF> extractor = SURF::create();
  extractor->compute( sceneMat, keypointsS, descriptors_scene );
  extractor->compute( objectMat, keypointsO, descriptors_object );

  FlannBasedMatcher matcher;  
  descriptors_scene.size(), keypointsO.size(), keypointsS.size();
  vector< vector< DMatch >  > matches;
  matcher.knnMatch( descriptors_object, descriptors_scene, matches, 2 );    
  vector< DMatch > good_matches;
  good_matches.reserve(matches.size());  
     
  for (size_t i = 0; i < matches.size(); ++i)
  { 
      if (matches[i].size() < 2)
                  continue;
     
      const DMatch &m1 = matches[i][0];
      const DMatch &m2 = matches[i][1];
     
      if(m1.distance <= 0.75 * m2.distance)        
      good_matches.push_back(m1);     
  }   
   
  if( (good_matches.size() >=match))
  	value = true;
       
    return value;
}


int main()
{
wiringPisetup();
int check=100;
bool isblockblue;
int arrow_dir;
bool techfest;
//input pin
int input_pin=7;
//output pins
int blue=12;
int red=18;
int left=23;
int right=8;
int isTechfestBlock=7;
int noTechfestBlock=24;
//pinmodes
pinMode(blue, OUTPUT);
pinMode(red, OUTPUT);
pinMode(left, OUTPUT);
pinMode(right, OUTPUT);
pinMode(isTechfestBlock, OUTPUT);
pinMode(noTechfestBlock, OUTPUT);
pinMode(input_pin, INPUT);
//default
digitalWrite(blue, LOW);
digitalWrite(red,LOW);
digitalWrite(right,LOW);
digitalWrite(left,LOW);
digitalWrite(isTechfestBlock,LOW);
digitalWrite(noTechfestBlock,LOW);
//detect blue block
while (digitalRead(input_pin)==1)
{
isblockblue= find_blue_Squares();
if (isblockblue== true) //signal from RPi to arduino that the block is blue
{
  digitalWrite(blue,HIGH);
  while (digitalRead(input_pin)==1)
  {
    arrow_dir= arrowDetect();
    if (arrow_dir==0)// right
    {
      digitalWrite(right, HIGH);
    }
    else if (arrow_dir==1)
    {
      digitalWrite(left, HIGH);
    }
    else if (arrow_dir==2)
    {
      continue;
    }
  }
}
 else if (isblockblue==false)
 {
  digitalWrite(red, HIGH)
  while (digitalRead(input_pin)==1)
  {
    arrow_dir= arrowDetect();
    if (arrow_dir==0)// right
    {
      digitalWrite(right, HIGH);
    }
    else if (arrow_dir==1)
    {
      digitalWrite(left, HIGH);
    }
    else if (arrow_dir==2)
    {
      continue;
    }
  }
 }
}
//run1 over
//now bot will detect techfest logo
while (digitalRead(input_pin)==1)
{
  techfest= logoDetect();
  if (techfest== true)
  {
    digitalWrite(isTechfestBlock,HIGH);
    check=0;
  }
  else if (techfest==false)
  {
    digitalWrite(noTechfestBlock, HIGH);
  }
}
while (digitalRead(input_pin)==0 && check=100)
{
  techfest= logoDetect();
  if (techfest== true)
  {
    digitalWrite(isTechfestBlock,HIGH);
    check=0;
  }
  else if (techfest==false)
  {
    digitalWrite(noTechfestBlock, HIGH);
  }
}
while (digitalRead(input_pin)==1 && check=100)
{
  techfest= logoDetect();
  if (techfest== true)
  {
    digitalWrite(isTechfestBlock,HIGH);
    check=0;
  }
  else if (techfest==false)
  {
    digitalWrite(noTechfestBlock, HIGH);
  }
}
}

