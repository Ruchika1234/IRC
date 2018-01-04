#include "opencv2/imgproc.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <math.h>
using namespace cv;
using namespace std;
int main()
{   //image->gray->center of circle->check around every pixel in the circle-> check percentage white.
	Mat src= imread("techfest.jpg"); 
	Mat gray,blur;
	double white=0;
	cvtColor(src, gray, CV_BGR2GRAY);
	GaussianBlur(gray, blur, Size(5,5), 0,0);
	vector<Vec3f>  circle;
	HoughCircles(blur, circle, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 100, 0, 0 );
	vector<Point> neighbours;
  /// Draw the circles detected
  for( size_t d = 0; d < circles.size(); d++ )
  {
        int radius = cvRound(circle[d][2]);
		neighbours.push_back(Point(circle[d][i][0], circle[d][i][1]));
		for (int j=1;j<circle[i][2];++j)
		{
			neighbours.push_back(Point(circle[d][i][0]-j, circle[d][i][1]));
			for (int h=1;h<=j;++h)
			{
				neighbours.push_back(Point(circle[d][i][0]-j, circle[d][i][1]+h));
				neighbours.push_back(Point(circle[d][i][0]-j, circle[d][i][1]-h));
			}
			neighbours.push_back(Point(circle[d][i][0]+j, circle[d][i][1]));
			for (int h=1;h<=j;++h)
			{
				neighbours.push_back(Point(circle[d][i][0]+j, circle[d][i][1]+h));
				neighbours.push_back(Point(circle[d][i][0]+j, circle[d][i][1]-h));
			}
			neighbours.push_back(Point(circle[d][i][0], circle[d][i][1]-j));
			for (int h=1;h<=j;++h)
			{
				neighbours.push_back(Point(circle[d][i][0]+h, circle[d][i][1]-j));
				neighbours.push_back(Point(circle[d][i][0]-h, circle[d][i][1]-j));
			}
			neighbours.push_back(Point(circle[d][i][0], circle[d][i][1]+j));
			for (int h=1;h<=j;++h)
			{
				neighbours.push_back(Point(circle[d][i][0]+h, circle[d][i][1]+j));
				neighbours.push_back(Point(circle[d][i][0]-h, circle[d][i][1]+j));
			}
		}
			}
		for (int f=0;f<neighbours.size();++f)
			{
			double intensity= blur.at<uchar>(neighbours[f].y, neighbours[f].x);
			//cout<<neighbours[f]<<endl;
			//circle(src, neighbours[f], 0.0000003, Scalar(0,255,0), 3, 8, 0);
				if (intensity<50.0)
				{
					white= white+1;
				}
			}
	imshow("win", src);
	//cout<<white;
	waitKey(0);
}
