#include "opencv2/imgproc.hpp"
#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

using namespace std;
int main()
{
Mat filled(260, 361, CV_8UC3,Scalar(255,255,255) );
Mat arrow(260, 361, CV_8UC3,Scalar(0,0,0) );
vector<Point> ppt;
int  y= 7;
int* p=&y;
Point P[7]={ Point(100,30), Point(200, 30), Point(200,20), Point(225, 50), Point(200,80), Point(200,70), Point(100,70) };
for(int a=0,b=1;a<7,b<=7;++a,++b)
{
if(b!=7)
{
line( arrow, P[a],P[b], Scalar(255,255,255), 1, 8, 0);
}
else
{
line( arrow, P[6], P[0], Scalar(255,255,255), 1, 8, 0);
}}
for(int i=0;i<7;++i)
{
ppt.push_back(P[i]);
}
fillConvexPoly (filled, ppt,  Scalar(0,0,255), 4,0);
namedWindow("win");
imshow("win", arrow);
namedWindow("win1");
imshow("win1", filled);
waitKey(0);
return 0;
}
