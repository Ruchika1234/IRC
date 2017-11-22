#include "opencv2/imgproc.hpp"
#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

using namespace std;
 int iLastX = -1; 
 int iLastY = -1;

int main()
{
VideoCapture stream1(0);
while (true) {
Mat cameraFrame;
stream1.read(cameraFrame);
Mat image= cameraFrame.clone();
Mat hsv;
cvtColor(image, hsv, CV_RGB2HSV);
Mat img;
inRange(hsv, Scalar(0,0,0), Scalar(175, 0, 0), img);
Moments oMoments = moments(img);

   double dM01 = oMoments.m01;
  double dM10 = oMoments.m10;
  double dArea = oMoments.m00;

   // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
  if (dArea > 10000)
  {
   //calculate the position of the ball
   int posX = dM10 / dArea;
   int posY = dM01 / dArea;        
        
   if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
   {
    //Draw a red line from the previous point to the current point
    line(img, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
   }

    iLastX = posX;
   iLastY = posY;
  }



 
namedWindow("win");
imshow("win", img);
if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break; 
       }
}
return 0;
}
