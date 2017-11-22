#include "opencv2/imgproc.hpp"
#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

using namespace std;
int main()
{
Mat arrow(260, 361, CV_8UC3,Scalar(0,0,0) );
Point P[7]={ Point(100,30), Point(200, 30), Point(200,20), Point(225, 50), Point(200,80), Point(200,70), Point(100,70) };
for(int i=0,j=1;i<7,j<=7;++i,++j)
{
if(j!=7)
{
line( arrow, P[i],P[j], Scalar(255,255,255), 1, 8, 0);
}
else
{
line( arrow, P[6], P[0], Scalar(255,255,255), 1, 8, 0);
}}
Mat gray;
cvtColor(arrow, gray, CV_BGR2GRAY);
blur(gray, gray, Size(3,3) );
Mat canny;
Canny( gray, canny, 100, 200, 3);
vector<vector<Point> > contours;
vector<Vec4i> hire;
findContours( canny, contours, hire, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
Mat drawing=Mat :: zeros(canny.size(), CV_8UC3);
for( int i = 0; i< contours.size(); i++ )
   {
       //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      drawContours( drawing, contours, i,(0,255,255),1, 8, hire, 0, Point() );
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
cout<<mu[i].m00<<endl;
}
for(int i=0;i< contours.size();++i)
{
cout<<mu[i].m10/mu[i].m00<<"\n"<<mu[i].m01/mu[i].m00<<endl;
}
for(int i=0;i<contours.size();++i)
{
circle(drawing, mc[i],6,(0,255,255), 1,8,0);
}
cout<<contours.size()<<endl;
namedWindow("win");
imshow("win", drawing);
waitKey(0);
//namedWindow("win");
//imshow("win", arrow);
//waitKey(0);
////Point centroid= Point( (150*1400+750*208.3)/2150, 50);
//if((150*1400+750*208.3)/2150> 225)
//{
//cout<<"left"<<endl;
//}
//else
//{
//cout<<"right"<<endl;
//}
return 0;
}
