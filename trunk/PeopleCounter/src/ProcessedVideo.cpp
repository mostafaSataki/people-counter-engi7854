/*
 * ProcessedVideo.cpp
 *
 *  Created on: 2011-07-20
 *      Author: Justin
 */

#include "ProcessedVideo.h"
#include <cv.h>
#include <highgui.h>
#include <iostream>

ProcessedVideo::ProcessedVideo(){
	cvNamedWindow("People Counter 'Processed Video'");
}

ProcessedVideo::~ProcessedVideo(){
	cvReleaseImage(&mCurrentFrame);
	cvDestroyWindow("People Counter 'Processed Video'");
}

void ProcessedVideo::displayVideo(){
	//tmp video display of an image
	mCurrentFrame = cvLoadImage("C:\\Users\\Justin\\Pictures\\Bleach\\1_bleach3.jpg");
	if(!mCurrentFrame){
		std::cout<< "Nothing in mCurrentFrame";
	}

	cvShowImage("People Counter 'Processed Video'", mCurrentFrame);
	cvWaitKey(0);
}
