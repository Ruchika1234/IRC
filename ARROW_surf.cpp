#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <math.h>
#include "opencv2/features2d.hpp"
//#include "opencv2/xfeatures2d.hpp"
//#include "opencv2/nonfree/xfeatures2d.hpp"
#include <string.h>
using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;
int main()
{
	VideoCapture cam(0);
	Mat frame;
	int dir;
	int avg_x, min_x,max_x;
		cam.read(frame);
    Mat image=frame.clone();
    Mat objectMat = imread("arrow_crop.jpg");
    int hessianValue = 1600;
    int match =30;
    bool value = false;
    while (true)
    {
   cam.read(frame);
    Mat sceneMat=frame.clone();
    vector< KeyPoint > keypointsO;
    vector< KeyPoint > keypointsS;    
 
    Mat descriptors_object, descriptors_scene;      

     Ptr<SURF> detector = SURF::create( hessianValue );
        detector->detect(sceneMat,keypointsS);
    if(keypointsS.size() < 5) return false; 
        detector->detect(objectMat,keypointsO);
    if(keypointsO.size() < 5) return false;
 
 
  Ptr<SURF> extractor = SURF::create();
  extractor->compute( sceneMat, keypointsS, descriptors_scene );
  extractor->compute( objectMat, keypointsO, descriptors_object );

  FlannBasedMatcher matcher;  
  descriptors_scene.size(), keypointsO.size(), keypointsS.size();
  vector< vector< DMatch >  > matches;
  matcher.knnMatch( descriptors_object, descriptors_scene, matches, 2 );    
  vector< DMatch > good_matches;
  good_matches.reserve(matches.size());  
     
  for (size_t i = 0; i < matches.size(); ++i)
  { 
      if (matches[i].size() < 2)
                  continue;
     
      const DMatch &m1 = matches[i][0];
      const DMatch &m2 = matches[i][1];
     
      if(m1.distance <= 0.75 * m2.distance)        
      good_matches.push_back(m1);     
  }   
  if( (good_matches.size() >=match))
  {
 
    cout << "OBJECT FOUND!" << good_matches.size()<<endl;
 
    std::vector< Point2f > obj;
    std::vector< Point2f > scene;
 
    for( unsigned int i = 0; i < good_matches.size(); i++ )
    {
      
        obj.push_back( keypointsO[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypointsS[ good_matches[i].trainIdx ].pt );
    }
 
    Mat H = findHomography( obj, scene, RANSAC );   
 
   
    std::vector< Point2f > obj_corners(4);
    obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( objectMat.cols, 0 );
    obj_corners[2] = cvPoint( objectMat.cols, objectMat.rows ); obj_corners[3] = cvPoint( 0, objectMat.rows );
    std::vector< Point2f > scene_corners(4);
 
    perspectiveTransform( obj_corners, scene_corners, H);
    Rect roi(scene_corners[1].x,scene_corners[1].y,scene_corners[3].x,scene_corners[3].y);
    Mat crop= objectMat(roi);
    Mat hsv, mask, dst;
    cvtColor(crop, hsv, CV_BGR2HSV);
	inRange(hsv,Scalar(0,0,0), Scalar(180,255,80),mask);
	GaussianBlur( mask, dst, Size( 5, 5 ), 0, 0 );
	vector<vector<Point> > contours;
		vector<Vec4i> hire;		
		findContours(dst, contours, hire, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		vector<vector<Point> > approx;
    	approx.resize(contours.size());
       	//approxPolyDP(contours[max_contour], approx, 3, true);
       	if (contours.size()!=0)
       	{
    	for (int d=0;d<approx.size();++d)
    	{
    		approxPolyDP(contours[d], approx[d], 3, true);
    if (approx[d].size()>=7 && approx[d].size()<=15)
       	{
       		double maxCosine = 0;
       		if (approx[d].size()>=7 && approx[d].size()<=15)
       	{
       		max_x=approx[d][0].x;
       		for (int g=1;g<approx[d].size();++g)
       		{
			//cout<<approx[d][g].x<<endl;
			circle(image, Point(approx[d][g].x,approx[d][g].y), 6,(0,255,255), 1,8,0);
       			if (max_x<approx[d][g].x)
       			{
       				max_x=approx[d][g].x;
       			}
       		}
       		min_x=approx[d][0].x;
       		for (int g=1;g<approx[d].size();++g)
       		{
       			if (min_x>approx[d][g].x)
       			{
       				min_x=approx[d][g].x;
       			}
       		}
       		avg_x=(max_x+min_x)/2;
       		vector<Moments> mu(contours[d].size() );
			for(int i=0; i<contours[d].size(); ++i)
			{
			mu[i]=moments(contours[d], false);
			}
			vector<Point2f> mc(contours[d].size() );
			for(int i=0;i<contours[d].size(); ++i)
			{
			mc[i]=Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
			}
			for (int h=0;h<mc.size();++h)
			{
			if(mc[h].x<avg_x)
			{ dir=0;
				cout<<"right"<<endl;
			}
			else
			{dir=1;
				cout<<"left"<<endl;
			
			}
      if (dir!=2)
      {
        break;
      }
		}
	}
}
if (waitKey(30) >= 0)
{
	break;
}  
}
}
}
if (waitKey(30) >= 0)
{
	break;
} 
}
}