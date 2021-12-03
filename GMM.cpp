// GMM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cv.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>

#include <opencv2/opencv.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>


using namespace cv;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	VideoCapture myvideo("./video.avi");
	int n_mixture=200, n_history=100, frame_counter = 10;
	bool use_shadow = false; 

	if(myvideo.isOpened())
	{
		Mat frame, bg, fg_mask, masked_image;
		BackgroundSubtractorMOG2 subtractor(n_history,n_mixture,use_shadow);

		while (myvideo.read(frame))
		{	
			subtractor.operator()(frame, fg_mask, 0.01);
			dilate(fg_mask,fg_mask,Mat());
			erode(fg_mask,fg_mask,Mat());

			subtractor.getBackgroundImage(bg);

			// get foreground mask
			for(int i=0; i<fg_mask.rows; i++)
			{
				for(int j=0; j<fg_mask.cols; j++)
				{
					if(fg_mask.at<uchar>(i,j) == 127)
					{
						fg_mask.at<uchar>(i,j) = 0;	
					}
				}
			}
		

			// time to mask original image 
			masked_image = frame.clone();
			for(int i=0; i<masked_image.rows; i++)
			{
				for(int j=0; j<masked_image.cols; j++)
				{
					if(fg_mask.at<uchar>(i,j) == 0)
					{
						masked_image.at<Vec3b>(i,j)[0] = 0;
						masked_image.at<Vec3b>(i,j)[1] = 0;	
						masked_image.at<Vec3b>(i,j)[2] = 0;	
					}
				}
			}
		}

		// show video
		imshow("original_video", frame);
		imshow("background", bg);
		imshow("fg_mask", fg_mask);
		imshow("masked_image", masked_image);	
		
		waitKey(5);

	}
	
	system("pause");
	return 0;
}
