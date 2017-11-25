#include "opencv2/imgproc.hpp"
#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

using namespace std;
int main()
{
Mat src=imread("ellipse.jpg");
Mat gray;
double b=0, a=0,c=0,h=0;
cvtColor(src, gray, CV_BGR2GRAY);
GaussianBlur(gray, gray, Size(9,9), 2,2 );
//blur(g=0ray, gray, Size(3,3) );
Mat canny;
Canny( gray, canny, 100, 200, 3);
vector<vector<Point> > contours;
vector<Vec4i> hire;
findContours( canny, contours, hire, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
Mat drawing=Mat :: zeros(canny.size(), CV_8UC3);
vector<RotatedRect>  mini(contours.size());
for(int i=0; i<contours.size(); ++i)
{
mini[i]= fitEllipse(Mat(contours[i] ) );
}
for(int i=0; i<contours.size();++i)
{
ellipse(drawing, mini[i], Scalar(0,0,255), 2, 8);
}
Point ref=Point(300,500);
for(int i=0;i<contours.size();++i)
{
a=mini[i].center.x+a;
b=mini[i].center.y+b;
h=abs(mini[i].center.x-ref.x)+h;
}
cout<<Point (a/contours.size(), b/contours.size() )<<"\t"<<h/contours.size()<<endl;
for(int i=0;i<contours.size();++i)
{
double angle= atan(abs(mini[i].center.x-ref.x)/abs(ref.y-mini[i].center.y));
c=c+angle;
cout<<angle<<endl;
}
cout<<c/contours.size();
namedWindow("win");
imshow("win", drawing);
waitKey(0);
return 0;
}
