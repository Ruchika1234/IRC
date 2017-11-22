#include "opencv2/imgproc.hpp"
#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

using namespace std;
int main()
{
Mat filled(500, 500, CV_8UC3,Scalar(0,0,0) );
vector<Point> ppt;
Point P[7]={ Point(100,30), Point(200, 30), Point(200,20), Point(225, 50), Point(200,80), Point(200,70), Point(100,70) };
for(int i=0;i<7;++i)
{
ppt.push_back(P[i]);
}
fillConvexPoly (filled, ppt,  Scalar(0,0,255), 4,0);
Mat hsv;
Mat thresh;
cvtColor(filled, hsv, CV_BGR2HSV);
inRange(hsv, Scalar(0,0,0), Scalar(0,0,255), thresh);
blur(thresh, thresh, Size(3,3) );
Mat canny;
Canny( thresh, canny, 100, 200, 3);
vector<vector<Point> > contours;
vector<Vec4i> hire;
findContours( canny, contours, hire, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
Mat drawing=Mat :: zeros(canny.size(), CV_8UC3);
for( int i = 0; i< contours.size(); i++ )
   {
       
      drawContours( drawing, contours, i,Scalar(0,0,255) , 1, 8, hire, 0, Point() );
     }
vector<Moments> mu(contours.size() );
for(int i=0; i<contours.size(); ++i)
{
mu[i]=moments(contours[i], false);
}
vector<Point2f> mc(contours.size() );
for(int i=0;i<contours.size(); ++i)
{
mc[i]=Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
}
for(int i=0;i<contours.size();++i)
{
circle(drawing, mc[i],6,(0,255,255), 1,8,0);
}
if(mu[0].m10/mu[0].m00<225)
{
cout<<"right"<<endl;
}

namedWindow("win1");
imshow("win1", drawing);
namedWindow("win");
imshow("win", thresh);
waitKey(0);
return 0;
}
