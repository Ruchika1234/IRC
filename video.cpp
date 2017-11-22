#include "opencv2/imgproc.hpp"
#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

using namespace std;
RNG rng(12345);
int main()
{
VideoCapture stream1(0);   //0 is the id of video device.0 if you have only one camera.
 
if (!stream1.isOpened()) { //check if video device has been initialised
cout << "cannot open camera";
}
while (true) {
Mat cameraFrame;
stream1.read(cameraFrame);
Mat image= cameraFrame.clone();
Mat gray;
cvtColor(image, gray, CV_BGR2GRAY);
blur(gray, gray, Size(3,3) );
Canny( gray, gray, 100, 200, 3);
vector<vector<Point> > contours;
vector<Vec4i> hire;
findContours( gray, contours, hire, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
Mat drawing=Mat :: zeros(gray.size(), CV_8UC3);
for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hire, 0, Point() );
     }
imshow("cam", drawing);
if (waitKey(30) >= 0)
{break;
} }
return 0;
}
