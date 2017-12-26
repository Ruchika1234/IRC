#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <math.h>
#include "opencv2/core.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/nonfree/xfeatures2d.hpp"
#include <wiringPi.h>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;
// global variables
int direction=2;
// square detect
VideoCapture cam(0);
Mat frame;
int stop= 500;
int tf =0,ntf=0;
int b=0,r=0;
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
   bool value;
   Mat mask,image1;
    inRange(image, Scalar(110,50,50), Scalar(130,255,255),mask);
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
                b=b+1;
                if (b==5)
                {
                value = true;}
else
  {
    r++;
    if(r==5 && b==0)
      value = false;
    else if(r==5 && b>1)
      value = true;
  }
		    }
        }
    }  
    return value;  
  }


// arrow detect
static int arrowDetect()
{
    cam.read(frame);
    Mat image=frame.clone();
		Mat hsv,mask,dst,zoom_out;
    int checkL=0, checkR=0;
    double max_x,avg_x,min_x,max_contour=0;
    cvtColor(image, hsv, CV_BGR2HSV);
    inRange(hsv,Scalar(0,0,0), Scalar(180,255,80),mask);
    GaussianBlur( mask, dst, Size( 5, 5 ), 0, 0 );
    pyrDown( dst, zoom_out, Size( dst.cols/2, dst.rows/2 ) );
    vector<vector<Point> > contours;
    vector<Vec4i> hire;   
    findContours(zoom_out, contours, hire, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    vector<vector<Point> > approx;
          for( int h=1;h<contours.size();++h)
          {
            if (contourArea(contours[max_contour])<contourArea(contours[h]))
            {
              max_contour=h;
            }
          }
      approx.resize(contours[max_contour].size());
      
      for (int d=0;d<approx.size();++d)
      {
        approxPolyDP(contours[max_contour], approx[d], 3, true);
      }
      for (int d=0;d<approx.size();++d)
      {
        if (approx[d].size()>=7 && approx[d].size()<=15)
        {
          double maxCosine = 0;
          if (approx[d].size()>=7 && approx[d].size()<=15)
        {
          double maxCosine = 0;
          for( int j = 2; j <approx[d].size() ; j++ )
            {     int check=0;                   
                double cosine = fabs(angle(approx[d][j%4], approx[d][j-2], approx[d][j-1]));
                maxCosine = MAX(maxCosine, cosine);
            if( maxCosine < 1)
            {
              check=check+1;
              if (check<=2)
              {
          max_x=approx[d][0].x;
          for (int g=1;g<approx[d].size();++g)
          {
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
        checkR=checkR+1;
        if(checkR==4)
        {
        direction=0;}//right
      }
      else if (mc[h].x>avg_x)
      {
        checkL=checkL+1;
        if (checkL==4)
        {
        direction=1;}//left
      
      }
      else if (mc[h].x==avg_x)
      {}
    if (direction!=2){break;}
    }
  }
}
if (direction!=2){break;}
}
}
}
if (direction!=2){break;}
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
        detector->detect(objectMat,keypointsO);
 
  Ptr<SURF> extractor = SURF::create();
  extractor->compute( sceneMat, keypointsS, descriptors_scene );
  extractor->compute( objectMat, keypointsO, descriptors_object );

  //if ( descriptors_scene.empty() )
    //  continue;

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
  { 
    tf =tf+1;
    if (tf==5)
      	value = true;
  }
  else
  {
    ntf++;
    if(ntf==5 && tf==0)
      value = false;
    else if(ntf==5 && tf>0)
      value = true;
  }
       
    return value;
}

void halt()
{
delay(stop);
}

int main()
{
wiringPiSetup();
int check=100;
bool isblockblue;
int arrow_dir;
bool techfest;
//all pins are according to wpi board
//input pin
int input_pin=0;
//output pins
int blue=2;
int red=3;
int left=12;
int right=13;
int isTechfestBlock=14;
int noTechfestBlock=23;
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
while (true)
{
while (digitalRead(input_pin)==0)
{
	halt();
}
while (digitalRead(input_pin)==1)
{
isblockblue= find_blue_Squares();
cout<<"yes-blue"<<endl;
if (isblockblue== true) //signal from RPi to arduino that the block is blue
{
  digitalWrite(blue,HIGH);
  //stopping the code 
  while (digitalRead(input_pin)==0)
{
  halt();
}
//signal to detect arrow direction
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
  digitalWrite(red, HIGH);
  while (digitalRead(input_pin)==1)
  {
    arrow_dir= arrowDetect();
    // stopping the code
    while (digitalRead(input_pin)==0)
{
  halt();
}
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
 while (digitalRead(input_pin)==0)
{
  halt();
}
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
/*while (digitalRead(input_pin)==0 && check==100)
{
  digitalWrite(noTechfestBlock, LOW);
}*/
 while (digitalRead(input_pin)==0)
{
  halt();
}
while (digitalRead(input_pin)==1 && check==100)
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
while (digitalRead(input_pin)==0)
{
  halt();
}
while (digitalRead(input_pin)==1 && check==100)
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
}