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
vector<Point> tri;
Point P[4]={ Point(100,30), Point(200, 30), Point(200,130), Point(100,130) };
Point PT[3]={Point(300, 30), Point(400, 30), Point(350,50)};
for(int i=0;i<4;++i)
{
ppt.push_back(P[i]);
}
for(int i=0;i<3;++i)
{
tri.push_back(PT[i]);
}

fillConvexPoly (filled, ppt,  Scalar(0,0,255), 4,0);
fillConvexPoly(filled, tri, Scalar(0,0,255), 4, 0);
Mat hsv;
Mat thresh;
cvtColor(filled, hsv, CV_BGR2HSV);
inRange(hsv, Scalar(0,0,0), Scalar(0,0,255), thresh);
blur(thresh, thresh, Size(3,3) );
Mat canny;
Canny( thresh, canny, 100, 200, 3);
vector<vector<Point> > contours;
vector<Vec4i> hire;
vector<Point> result;
findContours( canny, contours, hire, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
Mat drawing=Mat :: zeros(canny.size(), CV_8UC3);
for( int i = 0; i< contours.size(); i++ )
   {
       
      drawContours( drawing, contours, i,Scalar(0,0,255) , 1, 8, hire, 0, Point() );
}
result.resize(contours.size() );
for(int k=0;k<contours.size();++k)
{
approxPolyDP(contours[k], result, 3, true); 
for(int i=0;i<result.size();++i)
{
if(result.size()==4)
{
cout<<"yes";
for(int h=0;h<4;++h)
{
line(drawing, result[0], result[1], Scalar(255,0,0), 1,8,0);
line(drawing, result[1], result[2], Scalar(255,0,0), 1,8,0);
line(drawing, result[2], result[3], Scalar(255,0,0), 1,8,0);
line(drawing, result[3], result[0], Scalar(255,0,0), 1,8,0);
}
}
}
}
//}
//if(result.size()==4)
//{
//cout<<"yes";
//cout<<k;
//}
//}
for(int i=0;i<contours[0].size();++i)
{
circle(drawing, result[i],4,Scalar(0,255,0), 1,8,0);
}
namedWindow("win");
imshow("win", drawing);
waitKey(0);
return 0;
}
