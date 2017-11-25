#include "opencv2/imgproc.hpp"
#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

using namespace std;
int main()
{
Mat src=imread("circle.jpg");
Mat gray;
int h=0, a=0;
cvtColor(src, gray, CV_BGR2GRAY);
GaussianBlur(gray, gray, Size(9,9), 2,2 );
//blur(g=0ray, gray, Size(3,3) );
Mat canny;
Canny( gray, canny, 100, 200, 3);
vector<vector<Point> > contours;
vector<Vec4i> hire;
findContours( canny, contours, hire, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
Mat drawing=Mat :: zeros(canny.size(), CV_8UC3);
for( int i = 0; i< contours.size(); i++ )
   {
      // Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      drawContours( drawing, contours, i,(0,255,255),1, 8, hire, 0, Point() );
     }
vector<RotatedRect>  mini(contours.size());
for(int i=0; i<contours.size(); ++i)
{
mini[i]= fitEllipse(Mat(contours[i] ) );
}
for(int i=0; i<contours.size();++i)
{
ellipse(drawing, mini[i], Scalar(0,0,255), 2, 8);
}
for(int i=0;i<contours.size(); ++i)
{
if(mini[i].size.height-mini[i].size.width<0.06)
{
cout<<"circle"<<endl;
cout<<mini[i].size.height-mini[i].size.width<<endl;
cout<<mini[i].center<<endl;
}
}
cout<<mini.size();
//vector<Vec3f> circles;
//HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, gray.rows/8, 100, 200, 0, 10);
//for(int i=0;i<circles.size();++i)
//{
//circle(src, Point(circles[i][0], circles[i][1]), circles[i][2], Scalar(0,0,255), 3,8,0);
//}
//cout<<circles.size()<<endl;
//for(int i=0;i<circles.size();++i)
//{
//cout<<circles[i][2]<<endl;
//}
Point ref= Point( 300,640);
for(int i=0;i< mini.size();++i)
{
double  height= ref.x- mini[i].center.x;
cout<<height;
h=height+h;
}
cout<<h/mini.size()<<endl;
//cout<<circles[0][0]<<"\n"<<circles[0][1]<<endl;
for(int i=0;i<mini.size();++i)
{
double angle= atan(abs(mini[i].center.x-ref.x)/abs(ref.y-mini[i].center.y));
a=a+angle;
}
cout<<a/mini.size(); 
namedWindow("win");
namedWindow("win1");
imshow("win1", drawing);
imshow("win", src);
waitKey(0);
return 0;
}
