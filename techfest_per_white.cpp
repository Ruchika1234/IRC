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
	vector<vector<Point> > contours;
	vector<vector<Point> > approx;
	vector<Vec4i> hire;
	findContours(blur, contours, hire, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<RotatedRect>  mini;
	approx.resize(contours.size());
    	for (int d=0;d<approx.size();++d)
    	{
    		approxPolyDP(contours[d], approx[d], 3, true);
		if (approx[d].size()>4)
		{
	mini.push_back(fitEllipse(Mat(contours[d])));
	}
}

	//ellipse(src, mini[d], Scalar(0,0,255), 2, 8);
	for (int d=0;d<mini.size();++d)
	{
	if (abs(mini[d].size.height-mini[d].size.width<=5))
	{
		ellipse(src, mini[d], Scalar(0,0,255), 2, 8);
		vector<Point> neighbours;
			neighbours.push_back(Point(mini[d].center.x, mini[d].center.y));
		for (int j=1;j<mini[d].size.height && j<mini[d].size.width;++j)
		{
			neighbours.push_back(Point(mini[d].center.x-j, mini[d].center.y));
			for (int h=1;h<=j;++h)
			{
				neighbours.push_back(Point(mini[d].center.x-j, mini[d].center.y+h));
				neighbours.push_back(Point(mini[d].center.x-j, mini[d].center.y-h));
			}
			neighbours.push_back(Point(mini[d].center.x+j, mini[d].center.y));
			for (int h=1;h<=j;++h)
			{
				neighbours.push_back(Point(mini[d].center.x+j, mini[d].center.y+h));
				neighbours.push_back(Point(mini[d].center.x+j, mini[d].center.y-h));
			}
			neighbours.push_back(Point(mini[d].center.x, mini[d].center.y-j));
			for (int h=1;h<=j;++h)
			{
				neighbours.push_back(Point(mini[d].center.x+h, mini[d].center.y-j));
				neighbours.push_back(Point(mini[d].center.x-h, mini[d].center.y-j));
			}
			neighbours.push_back(Point(mini[d].center.x, mini[d].center.y+j));
			for (int h=1;h<=j;++h)
			{
				neighbours.push_back(Point(mini[d].center.x+h, mini[d].center.y+j));
				neighbours.push_back(Point(mini[d].center.x-h, mini[d].center.y+j));
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
}
}
	imshow("win", src);
	//cout<<white;
	waitKey(0);
}
